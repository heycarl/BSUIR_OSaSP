//
// Created by sasha on 5/17/23.
//

#ifndef OSASP_LABS_PRODUCER_H
#define OSASP_LABS_PRODUCER_H

#include "../message/message.h"

void create_producer(void);
void* produce_handler(void* arg);
void produce(msg_t *msg);
void remove_producer(void);

#endif //OSASP_LABS_PRODUCER_H
