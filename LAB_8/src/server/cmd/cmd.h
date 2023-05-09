//
// Created by sasha on 5/7/23.
//

#ifndef OSASP_LABS_CMD_H
#define OSASP_LABS_CMD_H

#include "echo.h"
#include "ack.h"
#include "info.h"
#include "fs.h"
#include "quit.h"

#include "command.h"

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
    void (*handler)(args_t*);
} cmd_t;

cmd_t cmds[NUM_OF_CMDs] = {
        {ECHO, "ECHO",cmd_echo},
        {QUIT, "QUIT", cmd_quit},
        {ACK, "ACK",cmd_ack},
        {INFO, "INFO", cmd_info},
        {CD, "CD", cmd_cd},
        {LIST, "LIST", cmd_list}
};

#endif //OSASP_LABS_CMD_H
