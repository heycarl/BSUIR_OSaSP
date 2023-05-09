//
// Created by sasha on 5/8/23.
//
#define BUFFER_SIZE 2048

#ifndef OSASP_LABS_COMMAND_H
#define OSASP_LABS_COMMAND_H

#include <stdint.h>
#include <limits.h>

typedef struct {
    int socket;
    char* buffer;
    size_t read_bytes;
    char current_path[PATH_MAX];
} args_t;

#define BUFFER_SIZE 2048

#endif //OSASP_LABS_COMMAND_H
