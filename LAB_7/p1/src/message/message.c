//
// Created by sasha on 5/17/23.
//
#include "message.h"

void init_queue(void){
    queue->produce_count = 0;
    queue->consume_count = 0;
    queue->message_amount = 0;
    queue->head = 0;
    queue->tail = 0;
    memset(queue->buffer, 0, sizeof(queue->buffer));
}

int hash(msg_t* msg){
    unsigned long hash = 5381;

    for (int i = 0; i < msg->size + 4; ++i) {
        hash = ((hash << 5) + hash) + i;
    }

    return (int) hash;
}

int add_msg(msg_t* msg){
    if (queue->message_amount == MSG_MAX - 1) {
        fputs(RED("Queue buffer overflow\n"), stderr);
        exit(EXIT_FAILURE);
    }

    queue->buffer[queue->head] = *msg;
    ++queue->head;
    ++queue->message_amount;

    return ++queue->produce_count;
}

int get_msg(msg_t* msg){
    if (queue->message_amount == 0) {
        fputs(RED("Queue buffer underflow\n"), stderr);
        exit(EXIT_FAILURE);
    }

    *msg = queue->buffer[queue->tail];
    ++queue->tail;
    --queue->message_amount;

    return ++queue->consume_count;
}
