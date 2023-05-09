//
// Created by sasha on 5/7/23.
//

#ifndef OSASP_LABS_INFO_H
#define OSASP_LABS_INFO_H

#include <sys/socket.h>

#include "command.h"

void cmd_info(args_t* args) {
    packet_t response;
    response.type = PACKET_INFO;
    char payload[BUFFER_SIZE];
    char hostname[HOST_NAME_MAX];
    gethostname(hostname, HOST_NAME_MAX);
    sprintf(payload, "Hello from %s server!", hostname);
    set_payload_string(&response, payload);
    send(args->socket, serialize_packet(&response), PACKET_BUFFER_SIZE, 0);
}

#endif //OSASP_LABS_INFO_H
