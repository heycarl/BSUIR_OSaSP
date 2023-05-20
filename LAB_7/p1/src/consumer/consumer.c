//
// Created by sasha on 5/17/23.
//

#include "consumer.h"
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>

#include "color_print.h"

extern pthread_mutex_t mutex;
extern pthread_cond_t cond_producer;
extern pthread_cond_t cond_consumer;
extern pthread_t consumers[];
extern int consumers_amount;

void create_consumer(void){
    if (consumers_amount == CHILD_MAX - 1) {
        fputs("Max value of consumers\n", stderr);
        return;
    }

    int res = pthread_create(&consumers[consumers_amount], NULL, consume_handler, NULL);
    if (res) {
        fputs(RED("Failed to create producer\n"), stderr);
        exit(res);
    }

    ++consumers_amount;
}

void* consume_handler(void* arg){
    msg_t msg;
    int counter;
    while(1){
        pthread_mutex_lock(&mutex);
        while (queue->message_amount == 0)
            pthread_cond_wait(&cond_consumer, &mutex);

        counter = get_msg(&msg);

        pthread_cond_signal(&cond_producer);
        pthread_mutex_unlock(&mutex);

        consume(&msg);

        pthread_t ptid = pthread_self();
        printf(CYAN("%d")"-c) %ld consume msg: hash=%X\n",
               counter, ptid, msg.hash);

        sleep(3);
    }
}

void consume(msg_t *msg) {
    int control_hash = hash(msg);
    if (msg->hash != control_hash) {
        fprintf(stderr, RED("control_hash=%X not equal msg_hash=%X\n"),
                control_hash, msg->hash);
    }
}

void remove_consumer(void){
    if (consumers_amount == 0) {
        fputs("Amount consumers = 0\n", stderr);
        return;
    }

    --consumers_amount;
    pthread_cancel(consumers[consumers_amount]);
    pthread_join(consumers[consumers_amount], NULL);
}
