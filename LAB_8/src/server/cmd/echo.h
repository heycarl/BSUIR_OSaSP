//
// Created by sasha on 5/7/23.
//

#ifndef OSASP_LABS_ECHO_H
#define OSASP_LABS_ECHO_H

#include <sys/socket.h>
#include <string.h>

#include "command.h"

void cmd_echo(args_t* args) {
    char* r = args->buffer + strlen("ECHO ") * sizeof(char);
    send(args->socket, r, strlen(r) * sizeof(char), 0);
}

#endif //OSASP_LABS_ECHO_H
