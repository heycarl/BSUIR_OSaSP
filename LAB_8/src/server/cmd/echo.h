////        buffer[read_size - 1] = '\0';

// Created by sasha on 5/7/23.
//

#ifndef OSASP_LABS_ECHO_H
#define OSASP_LABS_ECHO_H

#include <sys/socket.h>
#include <string.h>

#include "command.h"

void cmd_echo(args_t* args) {
    char* r = args->buffer + strlen("ECHO ") * sizeof(char);
    packet_t response;
    response.type = PACKET_INFO;
    set_payload_string(&response, r);
    send(args->socket, serialize_packet(&response), PACKET_BUFFER_SIZE, 0);
}

#endif //OSASP_LABS_ECHO_H
