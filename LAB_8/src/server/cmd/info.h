//
// Created by sasha on 5/7/23.
//

#ifndef OSASP_LABS_INFO_H
#define OSASP_LABS_INFO_H

#include <sys/socket.h>

#include "command.h"

void cmd_info(args_t* args) {
    char response[BUFFER_SIZE];
    strcpy(response, "Hello from LAB server!\n");
    send(args->socket, response, strlen(response), 0);
}

#endif //OSASP_LABS_INFO_H
