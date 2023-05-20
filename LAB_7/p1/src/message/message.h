//
// Created by sasha on 5/17/23.
//

#ifndef OSASP_LABS_MESSAGE_H
#define OSASP_LABS_MESSAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "color_print.h"

#define MSG_MAX 4096
#define CHILD_MAX 1024
#define SHARED_MEMORY_OBJ "/queue"

typedef struct {
    int type;
    int hash;
    int size;
    char data[256 * 3 + 1];
} msg_t;

typedef struct {
    int produce_count;
    int consume_count;
    int message_amount;
    int head;
    int tail;
    msg_t buffer[MSG_MAX];
} msg_struct;

extern msg_struct* queue;

void init_queue(void);
int hash(msg_t*);
int add_msg(msg_t* msg); //return produce count
int get_msg(msg_t* msg); //return extract count

#endif //OSASP_LABS_MESSAGE_H
