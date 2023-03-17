//
// Created by sasha on 2/16/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <wait.h>
#include <unistd.h>

#include "func.h"
#include "../keyboard_input.h"

extern char** environ;
extern const char child_path[];

int main(int argc, char* argv[], char* envp[]) {
    if (argc != 2) {
        fprintf(stderr, "%d is invalid amount of arguments, must be 2\n", argc);
        exit(EXIT_FAILURE);
    }

    print_env(envp);

    size_t child_count = 0;
    int opt;
    fix_keyboard();
    while ((opt = getchar()) != EOF) {
        if (opt == 'q') {
            exit(EXIT_SUCCESS);
        }
        if (opt != '+' && opt != '*' && opt != '&') {
            continue;
        }

        // get child executable path
        char* child_process = NULL;
        switch ((char) opt) {
            case '+':
                child_process = getenv(child_path);
                break;

            case '*':
                child_process = get_child_path(envp);
                break;

            case '&':
                child_process = get_child_path(environ);
                break;
        }

        // generate new child name
        char child_name[10];
        sprintf(child_name, "child_%zu", child_count++);
        char* const args[] = {child_name, argv[1]};

        pid_t pid = fork();
        if (pid > 0) {
            // Parent process
            int status;
            wait(&status);
        } else if (pid == 0) {
            // Child process
            CLEAR_SCREEN
            if (execve(child_process, args, envp) == -1) {
                perror("execve");
                exit(errno);
            }

        } else {
            perror("fork");
            exit(errno);
        }
    }
    restore_keyboard();
}
