//
// Created by sasha on 5/17/23.
//

#ifndef OSASP_LABS_CONSUMER_H
#define OSASP_LABS_CONSUMER_H

#include "../message/message.h"

void create_consumer(void);
void* consume_handler(void* arg);
void consume(msg_t *msg);
void remove_consumer(void);

#endif //OSASP_LABS_CONSUMER_H
