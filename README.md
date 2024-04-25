# mysh
# a simple command-line shell 


# Project Structure
  - My project is structured like so:
       - p3.c - contains functions for executing the required commands likke: tokenize_buffer(), executeBasic(), executePiped(), executeAsync(), executeRedirect(), and some helper functions.
       - test.c - contains functions for testing the functions defined in the p3.c file like: test_basic_commands(), test_io_redirection(), test_piping(), test_async_commands(), test_cd(), which_cd(), test_conditional().
       - README.md - file containing the testing plan and instructions on how the program works 
       - Makefile - compiles both p3.c and README.md files
            - How to Compile and Run:
                 - make p3 : to compile both files
                 - make run : to run p3.c file  
                 - ./p3
                 - make test : to run p3.c file
                 - ./test

# Design Decisions:
Error handling: The test functions use fprintf(stderr, ...) to print error messages if a system command fails. This ensures that errors are logged and can be easily identified.
Modularity: Each test function is modularized to focus on a specific aspect of the system commands being tested, such as basic commands, I/O redirection, piping, asynchronous commands, the cd command, the which command, and conditional commands.

Test Plan:         
* Test Plan: We ran a functional testing that aims to test each functionality  that might exit in a command:
  
Functional testing:

1. Basic Commands Test:
   - Description: Tests the execution of basic shell commands (ls, pwd, echo Hello, World!, exit).
   - Arguments: None.
   - Test Program: run `test_basic_commands()` in `test.c`.
   * How it works: Tests basic shell commands by Calling the `system()` function. Each command is executed, and the output is captured and printed to the console. If a command fails (returns -1), an error message is printed using fprintf(stderr, ...).

2. I/O Redirection Test:
   - Description:  Tests the I/O redirection capabilities of the shell.
   - Arguments: None.
   - Test Program: run `test_io_redirection()` in `test.c`.
   * How it works: tests commands containing either `<` or `>` operators which use the `system()` function to execute a command that redirects input/output. Otherwise it returns an error message.


3. Piping Test:
   - Description: Tests the piping functionality of the shell.
   - Arguments: None.
   - Test Program: run `test_piping()` in `test.c`.
   * How it works: test the working of a command that Uses the `|` operator to execute a series of commands that combine various shell functionalities, such as basic commands, I/O redirection, piping, etc. Otherwise it returns an error message.

4. Asynchronous Commands Test:
   - Description:  Tests the execution of asynchronous commands.
   - Arguments: None.
   - Test Program: run `test_async_commands` in `test.c`.
    * How it works: test the working of a command that Uses the `&` operator at the end of a command. The command is started in a separate process, allowing the shell to continue accepting input and executing other commands. Otherwise it returns an error message.

5. cd Command Test:
   - Description: Tests the change directory (cd) command.
   - Arguments: None.
   - Test Program: run `test_cd` in `test.c`.
   * How it works: test the working of a command starting by `cd` that receives a directory path as an argument, Changes the current working directory to the specified path if it exists and the shell has permission to access it. Otherwise it returns an error message.

6. which Command Test:
   - Description: Tests which command that prints the path used if asked to start the program.
   - Arguments: None.
   - Test Program: run `test_which` in `test.c`.
   * How it works: test the working of a command starting by `which` that Receives the name of a program as an argument, searches the directories listed in the PATH environment variable for an executable file with the specified name.Prints the full path to the executable file if found, or nothing if the program is not found. Otherwise it returns an error message.


7. Conditionals command test:
   - Description: Allocate, deallocate, and reallocate memory with value assignments.
   - Arguments: None.
   - Test Program: run `test_conditional()` in `test.c`.
   * How it works: Uses the `next` or `then` keyword to conditionally execute a command. Otherwise it returns an error message.



# Instuctions on how to use the program
mysh is a basic Unix shell implementation written in C similar to
bash or zsh. It provides interacrive and batch modes and provide
functionalities for executing certain commands and assume the following:
- executing basic commands: cd for changing the current directory, pwd for
printing the current working directory, which for the  path that mysh
would use if asked to start that program, and exit for exiting the
interactive mode.
- handling wildcards: used to represent and identify a set of files whose
names match a pattern. So, when a command includes a zildcard token '*'
it is replaced in the argument list by the list of names matching the
pattern. If no names match the pattern, the token is passed to the command
unchanged.
e.g. foo*bar matches file names in the working directory that begin with “foo” and end with
“bar”.
- handling I/O Redirection: allows the user to specify files to use as
standard input '<' and standard output '>'.
e.g. foo quux < bar > baz
- handling pipes: allowing creation of a single pipe that connects 
standard input from one program to the standard output of another,
enabling data flow between two processes.
e.g. foo | bar < baz 
- handling conditionals: allow checking the exit status of the previous
command and executing commands that contains 'then' and/or 'else'.
A command beginning with
'then' is only executed if the previous command succeeded, and a command
beginning with 'else' is only executed if the previous command failed. 
e.g. 
foo
then bar
else baz

# Installation Instructions
1. Clone the repository containing the source code in your machine.
2. Use a text editor such as Visual Studio Code
3. Make sure to install C compiler such as GCC, MSVC... 


# Find a bug
- If you found an issue or would like to submit an improvement to this
project, please submit an issue using the issues tab above.

# How to contribute
Please note that this project is fully made by me. If you would like to 
contribute to its improvement, consider submitting a PR with a fix.
