//
// Created by sasha on 2/16/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "color_print.h"

const int MAX_SIZE = 256;

int main(int argc, char* argv[]) {

    if (argc != 2) {
        fprintf(stderr, "%d is invalid arguments amount, must be 2\n", argc);
        exit(EXIT_FAILURE);
    }

    puts("Child process data:");
    printf(GREEN("Name: ") "%s\n", argv[0]);
    printf(GREEN("PID: ") "%d\n", getpid());
    printf(GREEN("Parent PID: ") "%d\n", getppid());

    char buffer[MAX_SIZE];
    FILE* file_envp = fopen(argv[1], "r");
    if (!file_envp) {
        perror("file_envp");
        exit(errno);
    }
    while (fgets(buffer, MAX_SIZE, file_envp) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
        printf(CYAN("%s") " = %s\n", buffer, getenv(buffer));
    }

    fclose(file_envp);
    exit(EXIT_SUCCESS);
}
