//
// Created by sasha on 5/7/23.
//

#ifndef OSASP_LABS_ECHO_H
#define OSASP_LABS_ECHO_H

#include <sys/socket.h>
#include <string.h>

void cmd_echo(int socket, char* buffer) {
    char* r = buffer + strlen("ECHO ") * sizeof(char);
    send(socket, r, strlen(r) * sizeof(char), 0);
}

#endif //OSASP_LABS_ECHO_H
