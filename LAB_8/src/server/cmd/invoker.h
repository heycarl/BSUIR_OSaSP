//
// Created by sasha on 5/7/23.
//

#ifndef OSASP_LABS_INVOKER_H
#define OSASP_LABS_INVOKER_H

#include "cmd.h"

void invoke(enum CMD command, int socket, char* buffer) {
    for (int i = 0; i < NUM_OF_CMDs - 1; ++i) {
        if (cmds[i].cmd == command){
            cmds[i].handler(socket, buffer);
            return;
        }
    }
}

#endif //OSASP_LABS_INVOKER_H
