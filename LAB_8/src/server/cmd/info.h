//
// Created by sasha on 5/7/23.
//

#ifndef OSASP_LABS_INFO_H
#define OSASP_LABS_INFO_H

#include <sys/socket.h>

void cmd_info(int socket, char* buffer) {
    send(socket, "Hello from LAB server!", sizeof("Hello from LAB server!"), 0);
}

#endif //OSASP_LABS_INFO_H
