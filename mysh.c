#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <glob.h>
#include <unistd.h>


//redirection modes
#define INPUT 0
#define OUTPUT 1
#define APPEND 2

// path array containing possible directories where the shell should search for the executable files
char *path[] = {"/usr/local/bin/", "/usr/bin/", "/bin/", NULL};
char command[1024];


//This function tokeniwes a buffer, expands wildcards, and stores the
//resulting tokens or filenames in an array and updating the count of tokens.
int tokenize_buffer(char **param, int *nr, char *buf, const char *c) {
    char *token;
    //c: a delimiter of type character
    //nr: return final number of tokens= filenames + original tokens 
    //param: array of strings that will store the expanded filenames in matching_filenames
    //extract tokens from a buffer
    char *saveptr;
    int pc = -1;

    token = strtok_r(buf, c, &saveptr);

    while (token) {
        if (strcmp(token, "then") == 0 || strcmp(token, "else") == 0) {
            token = strtok_r(NULL, c, &saveptr); // Skip over then/else
            if (token == NULL) {
                return 1; // Syntax error: missing command after then/else
            }
        }

        // Check if the token contains a wildcard
        if (strchr(token, '*')) {  //if yes
            glob_t matching_filenames; //creating a variable contain the filena;es that ;atches the pattern
            glob(token, 0, NULL, &matching_filenames); ////glob() function  expands the wildcard into matching filenames and stores the expanded filenames in matching_filenames
            //iterate each matching_filename and storing it in param
            for (int i = 0; i < matching_filenames.gl_pathc; i++) {
                param[++pc] = malloc(strlen(matching_filenames.gl_pathv[i]) + 1);
                strcpy(param[pc], matching_filenames.gl_pathv[i]);
            }
            globfree(&matching_filenames);
        } else {
            param[++pc] = malloc(strlen(token) + 1);
            strcpy(param[pc], token);
        };
        token = strtok_r(NULL, c, &saveptr);
    };

    param[++pc] = NULL;
    *nr = pc;

    return 0;
};

//iterates over an array of strings and prints each string until it reaches the end of the array.
void print_params(char **param) {
    while (*param) {
        printf("param=%s..\n", *param++);
    };
};

//executing commands existing in the buf array
void executeBasic(char **argv) {
    if (fork() > 0) {
        wait(NULL);
    } else {
        // Check if the command contains a slash or is a built-in command, if so it executes the command and returns an error in case of failure
        if (strchr(argv[0], '/') || strcmp(argv[0], "cd") == 0 || strcmp(argv[0], "exit") == 0) {
            execvp(argv[0], argv);
            perror("invalid input");
            exit(1);
        // if command does not contain a slash or not a built-in command, this for loop searches for the command in the specified directories in the path variable
        } else {
            for (int i = 0; path[i] != NULL; i++) {
                // for each path in the path variable, it constructs a full path of the executqble file by appending pqth to the command in buf[0] 
                snprintf(command, sizeof(command), "%s%s", path[i], argv[0]);
                if (access(command, X_OK) == 0) {  //access() checks if the path constructed is executable, if so, it is executed using execv()
                    execv(command, argv);
                    perror("invalid input");
                    exit(1);
                };
            };
        };
        printf("Command not found: %s\n", argv[0]);
    };
};


void executePiped(char **buf, int nr) {
    if (nr != 2) {
        printf("This function supports only one pipe. \n");
        return;
    };

    int file_descriptors[2], pc;
    char *argv[100]; //an array argv of strings to hold the command and its arguments.

    tokenize_buffer(argv, &pc, buf[0], " "); // Tokenizes the first command which is stored in buf[0]
    
    //creates a pipe and checks if creation was successful, otherwise returns error message and stop executing
    if (pipe(file_descriptors) < 0) {
        perror("pipe creating was not successful\n");
        return;
    };

    // Forks a child process to execute the first command
    if (fork() == 0) {
        dup2(file_descriptors[1], 1);
        close(file_descriptors[0]);
        close(file_descriptors[1]);
        execvp(argv[0], argv);
        perror("invalid input");
        exit(1);
    };

    tokenize_buffer(argv, &pc, buf[1], " ");  // Tokenizes the second  command which is stored in buf[1]
    if (fork() == 0) {
        dup2(file_descriptors[0], 0);
        close(file_descriptors[0]);
        close(file_descriptors[1]);
        execvp(argv[0], argv);
        perror("invalid input");
        exit(1);
    };

    close(file_descriptors[0]);
    close(file_descriptors[1]);
    wait(NULL);
    wait(NULL);
};

//executing multiple commands asynchronously
void executeAsync(char **buf, int nr) {
    int i, number_of_param;
    char *argv[100];
    // iterates over each command in the buf arrat
    for (i = 0; i < nr; i++) {
        // tokenizes the command number i from buf array and stors them in arg string
        tokenize_buffer(argv, &number_of_param, buf[i], " ");
        //forks a child process to excecute the current command
        if (fork() == 0) {
            execvp(argv[0], argv); //execute the current command along with its arguments
            perror("invalid input");
            exit(1);
        };
    };
    //parent process waiting for all child processes to complete executing
    for (i = 0; i < nr; i++) {
        wait(NULL);
    };
};

//executing a command with I/O redirection
void executeRedirect(char **buf, int nr, int mode) {
    int number_of_param, file_descriptor;
    char *argv[100];
    //Tokenizes the first command and stores it in buf[0]
    tokenize_buffer(argv, &number_of_param, buf[0], " ");
    if (fork() == 0) {
        switch (mode) {
            //case input redirection, it opens the file in the buff[1] for reading only 
            case INPUT:
                file_descriptor = open(buf[1], O_RDONLY);
                break;
            //case output redirection, it opens the file in the buff[1] for writing only with the capacity to create it if it does not exit or truncating it.
            case OUTPUT:
                file_descriptor = open(buf[1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
                break;
            //case output redirection, it opens the file in the buff[1] for writing only with the capacity to create it if it does not exit or appending data to the end of it. 
            case APPEND:
                file_descriptor = open(buf[1], O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
                break;
            default:
                return;
        };

        // checking if file descriptor is valid
        if (file_descriptor < 0) {
            perror("cannot open file\n");
            return;
        };
        //redirecting I/O based on redirection mode 
        switch (mode) {
            case INPUT:
                dup2(file_descriptor, 0);
                break;
            case OUTPUT:
            case APPEND:
                dup2(file_descriptor, 1);
                break;
            default:
                return;
        }
        execvp(argv[0], argv);
        perror("invalid input");
        exit(1);
    }
    wait(NULL);
}

int main(int argc, char **argv) {
    char buf[500], *buffer[100], cwd[1024];
    int nr = 0;
    int interactive = isatty(STDIN_FILENO);
    ssize_t bytes_read;

    if (interactive) {
        printf("Welcome to my shell!\n");
    };


    while (1) {
        printf("mysh> ");


        //Getting current working directory when typing pwd
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s ", cwd);
        } else {
            perror("unable to return cwd\n");
        };

        //Taking user input and storing it in buf string
        bytes_read = read(STDIN_FILENO, buf, sizeof(buf));

        if (bytes_read <= 0) {
            perror("read failed\n");
            return 1;
        }

        buf[bytes_read - 1] = '\0';

        // exiting when typing exit
        if (strcmp(buf, "exit") == 0) {
            break;
        };

	//interpreting command using tokenize_buffer() function based on tokens like | for pipes, & for asynchronous commands, >> for append, < or > for redirect
        if (strchr(buf, '|')) {
            tokenize_buffer(buffer, &nr, buf, "|");
            executePiped(buffer, nr);
        } else if (strchr(buf, '&')) {
            tokenize_buffer(buffer, &nr, buf, "&");
            executeAsync(buffer, nr);
        } else if (strstr(buf, ">>")) {
            tokenize_buffer(buffer, &nr, buf, ">>");
            if (nr == 2) {
                executeRedirect(buffer, nr, APPEND);
            } else {
                printf("Incorrect output redirection! Try again\n");
            }
        } else if (strchr(buf, '>')) {
            tokenize_buffer(buffer, &nr, buf, ">");
            if (nr == 2) {
                executeRedirect(buffer, nr, OUTPUT);
            } else {
                printf("Incorrect output redirection! Try again\n");
            }
        } else if (strchr(buf, '<')) {
            tokenize_buffer(buffer, &nr, buf, "<");
            if (nr == 2) {
                executeRedirect(buffer, nr, INPUT);
            } else {
                printf("Incorrect input redirection! Try again\n");
            }
        } else {
            tokenize_buffer(buffer, &nr, buf, " ");
            if (strcmp(buffer[0], "cd") == 0) {
                chdir(buffer[1]);
            } else if (strcmp(buffer[0], "which") == 0) {
                // Check if the which command has the correct number of arguments
                if (*buffer[1] == '\0') {
                    printf("second argument is missimg!\n");
                    return 1;
                };
                int i;
                for (int i = 0; path[i] != NULL; i++) {
                    snprintf(command, sizeof(command), "%s%s", path[i], buf[1]);
                    if (access(command, X_OK) == 0) {
                        printf("%s\n", command);
                        break;
                    }
                }
                if (path[i] == NULL) {
                    printf("Command not found: %s\n", buf[1]);
                };
            } else {
                executeBasic(buffer);
            };
        };
    };

    if (interactive) {
        printf("Exiting my shell.\n");
    };

    return 0;
};
