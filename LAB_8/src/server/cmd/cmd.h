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
    CMD_ECHO,
    CMD_QUIT,
    CMD_INFO,
    CMD_CD,
    CMD_LIST,
    CMD_ACK
};

typedef struct{
    enum CMD cmd;
    char cmd_name[255];
    void (*handler)(args_t*);
} cmd_t;

cmd_t cmds[NUM_OF_CMDs] = {
        {CMD_ECHO, "ECHO", cmd_echo},
        {CMD_QUIT, "QUIT", cmd_quit},
        {CMD_ACK,  "ACK",      cmd_ack},
        {CMD_INFO, "INFO", cmd_info},
        {CMD_CD,   "CD",   cmd_cd},
        {CMD_LIST, "LIST", cmd_list}
};

#endif //OSASP_LABS_CMD_H
