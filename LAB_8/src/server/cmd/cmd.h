//
// Created by sasha on 5/7/23.
//

#ifndef OSASP_LABS_CMD_H
#define OSASP_LABS_CMD_H

#include "echo.h"
#include "ack.h"

#define NUM_OF_CMDs 6

enum CMD {
    ECHO,
    QUIT,
    INFO,
    CD,
    LIST,
    ACK
};

typedef struct{
    enum CMD cmd;
    char cmd_name[255];
    void (*handler)(int socket, char* buffer);
} cmd_t;

cmd_t cmds[NUM_OF_CMDs] = {
        {ECHO, "ECHO",cmd_echo},
        {ACK, "ACK",cmd_ack},
};

#endif //OSASP_LABS_CMD_H
