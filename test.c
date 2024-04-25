#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


//This functional test code aims at testing each functionality of the program separeredly
// and if system() function return -1, it means that there is a problem with the command and 
//and the testing code returns an error message, otherwise it executes correctly.

void test_basic_commands() {
    printf("Testing basic commands:\n");
    if (system("ls") == -1) {
        fprintf(stderr, "Error executing ls \n");
    };
    if (system("pwd") == -1) {
        fprintf(stderr, "Error executing comman pwd\n");
    };
    if (system("echo Hello, World!") == -1) {
        fprintf(stderr, "Error executing echo Hello, World!\n");
    };
    if (system("exit") == -1) {
        fprintf(stderr, "Error executing exiting\n");
    };
};

void test_io_redirection() {
    printf("\nTesting input redirection:\n");
    if (system("echo This is a test < test.txt") == -1) {
        fprintf(stderr, "Error executing echo This is a test < test.txt\n");
    };
    printf("\nTesting output redirection:\n");
    if (system("echo This is a test > test.txt") == -1) {
        fprintf(stderr, "Error executing echo This is a test > test.txt\n");
    };
};

void test_piping() {
    printf("\nTesting piping:\n");
    if (system("ls | grep test") == -1) {
        fprintf(stderr, "Error executing ls | grep test\n");
    };
};

void test_async_commands() {
    printf("\nTesting asynchronous commands:\n");
    if (system("echo hello & echo world") == -1) {
        fprintf(stderr, "Error executing echo hello & echo world\n");
    };
};

void test_cd() {
    printf("\nTesting cd command:\n");
    if (system("cd /tmp && pwd") == -1) {
        fprintf(stderr, "Error executing cd /tmp && pwd\n");
    };
};
void which_cd() {
    printf("\nTesting which command:\n");
    if (system("which ls") == -1) {
        fprintf(stderr, "Error executing which command\n");
    };
};
void test_conditional() {
    printf("\nTesting else command:\n");
    if (system("echo hello else echo world") == -1) {
        fprintf(stderr, "Error executing echo hello else echo world\n");
    };
    printf("\nTesting then command:\n");
    if (system("echo hello then echo world") == -1) {
        fprintf(stderr, "Error executing echo hello then echo world\n");
    };
};

int main() {
    test_basic_commands();
    test_io_redirection();
    test_piping();
    test_async_commands();
    test_cd();
    test_which();
    test_conditional();

    return 0;
};