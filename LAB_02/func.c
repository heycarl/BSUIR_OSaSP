//
// Created by sasha on 2/16/23.
//

#include "func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "color_print.h"

const char child_path[] = "CHILD_PATH";
const int MAX_SIZE = 256;

static int qsort_cmp(const void* str1, const void* str2) {
    return strcmp(*(const char**) str1, *(const char**) str2);
}

void print_env(char* envp[]) {
    size_t envpc = 0;
    while (envp[envpc]) {
        ++envpc;
    }

    qsort(envp, envpc, sizeof(char*), qsort_cmp);

    printf(GREEN("Parent environment variables:") "\n");
    for (size_t i = 0; i < envpc; ++i) {
        printf(CYAN("%s\n"), envp[i]);
    }
}

//char** make_child_env(char* file_envp) {
//    FILE* stream = fopen(file_envp, "r");
//    if (!stream) {
//        perror("fopen");
//        exit(errno);
//    }
//
//    char** env = malloc(sizeof(char*));
//    size_t i = 0;
//
//    char buf[MAX_SIZE];
//
//    while (fgets(buf, MAX_SIZE, stream) != NULL) {
//        buf[strcspn(buf, "\n")] = '\0';
//
//        char* env_param = getenv(buf);
//        if (env_param) {
//            env[i] = malloc((strlen(buf) + strlen(env_param) + 2) * sizeof(char));
//            strcat(strcat(strcpy(env[i], buf), "="), env_param);
//            env = realloc(env, (++i + 1) * sizeof(char*));
//        }
//    }
//    env[i] = NULL;
//
//    return env;
//}

char* get_child_path(char** str_arr) {
    while (*str_arr) {
        if ( strstr(*str_arr, child_path) )
            return *str_arr + strlen(child_path) + 1; // move string head to string + var name + equal sign
        ++str_arr;
    }
    return NULL;
}
