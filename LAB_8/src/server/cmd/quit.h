//
// Created by sasha on 5/8/23.
//

#ifndef OSASP_LABS_QUIT_H
#define OSASP_LABS_QUIT_H

#include <sys/socket.h>

#include "command.h"

void cmd_quit(args_t* args) {
    packet_t response;
    response.type = PACKET_INFO;
    set_payload_string(&response, "BYE");
    send(args->socket, serialize_packet(&response), PACKET_BUFFER_SIZE, 0);
}
#endif //OSASP_LABS_QUIT_H
