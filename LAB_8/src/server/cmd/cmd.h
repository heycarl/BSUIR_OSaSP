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

#define NUM_OF_CMDs 7

enum CMD {
    CMD_ECHO,
    CMD_QUIT,
    CMD_INFO,
    CMD_CD,
    CMD_LIST,
    CMD_ACK,
    CMD_HELP
};

typedef struct{
    enum CMD cmd;
    char cmd_name[255];
    void (*handler)(args_t*);
} cmd_t;

void cmd_help(args_t*args);

cmd_t cmds[NUM_OF_CMDs] = {
        {CMD_ECHO, "ECHO", cmd_echo},
        {CMD_QUIT, "QUIT", cmd_quit},
        {CMD_ACK,  "ACK",      cmd_ack},
        {CMD_INFO, "INFO", cmd_info},
        {CMD_CD,   "CD",   cmd_cd},
        {CMD_LIST, "LIST", cmd_list},
        {CMD_HELP, "?", cmd_help}
};

void cmd_help(args_t*args) {
    packet_t response;
    response.type = PACKET_INFO;
    char buffer[BUFFER_SIZE];
    strcat(buffer, "Avaliable commands: \n");
    for (int i = 0; i < NUM_OF_CMDs; ++i) {
        strcat(buffer, cmds[i].cmd_name);
        strcat(buffer, "\n");
    }
    set_payload_string(&response, buffer);
    send(args->socket, serialize_packet(&response), PACKET_BUFFER_SIZE, 0);
}

#endif //OSASP_LABS_CMD_H
