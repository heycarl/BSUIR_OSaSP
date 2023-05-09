//
// Created by sasha on 5/8/23.
//

#ifndef OSASP_LABS_QUIT_H
#define OSASP_LABS_QUIT_H

#include <sys/socket.h>

#include "command.h"

void cmd_quit(args_t* args) {
    send(args->socket, "Bye!", sizeof("Bye!"), 0);
}
#endif //OSASP_LABS_QUIT_H
