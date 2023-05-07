//
// Created by sasha on 5/7/23.
//

#ifndef OSASP_LABS_PARSER_H
#define OSASP_LABS_PARSER_H
#include <string.h>

#include "cmd.h"

enum CMD parse_request(char* buffer) {
    for (int i = 0; i < NUM_OF_CMDs - 1; ++i) {
        if (strstr(buffer, cmds[i].cmd_name) != NULL)
            return cmds[i].cmd;
    }
    return -1;
}

#endif //OSASP_LABS_PARSER_H
