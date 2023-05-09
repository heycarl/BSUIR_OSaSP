//
// Created by sasha on 5/7/23.
//

#ifndef OSASP_LABS_ACK_H
#define OSASP_LABS_ACK_H

#include <sys/socket.h>

#include "command.h"

void cmd_ack(args_t*args) {
    send(args->socket, "ACK received\n", sizeof("ACK received\n"), 0);
}

#endif //OSASP_LABS_ACK_H
