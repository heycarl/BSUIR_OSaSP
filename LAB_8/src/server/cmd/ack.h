//
// Created by sasha on 5/7/23.
//

#ifndef OSASP_LABS_ACK_H
#define OSASP_LABS_ACK_H

#include <sys/socket.h>

void cmd_ack(int socket, char* buffer) {
    send(socket, "ACK received", sizeof("ACK received"), 0);
}

#endif //OSASP_LABS_ACK_H
