//
// Created by sasha on 5/7/23.
//

#ifndef OSASP_LABS_ACK_H
#define OSASP_LABS_ACK_H

#include <sys/socket.h>

#include "command.h"

void cmd_ack(args_t*args) {
    packet_t response;
    response.type = PACKET_ACK;
    set_payload_string(&response, "Recieved");
    send(args->socket, serialize_packet(&response), PACKET_BUFFER_SIZE, 0);
}

#endif //OSASP_LABS_ACK_H
