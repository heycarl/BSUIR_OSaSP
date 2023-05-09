//
// Created by sasha on 5/7/23.
//

#ifndef OSASP_LABS_INVOKER_H
#define OSASP_LABS_INVOKER_H

#include "cmd.h"
#include "command.h"

void invoke(enum CMD command, args_t* args) {
    for (int i = 0; i < NUM_OF_CMDs; ++i) {
        if (cmds[i].cmd == command){
            cmds[i].handler(args);
            return;
        }
    }
}

#endif //OSASP_LABS_INVOKER_H
