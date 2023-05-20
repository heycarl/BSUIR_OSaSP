//
// Created by sasha on 5/17/23.
//

#include "producer.h"
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>


extern pthread_mutex_t mutex;
extern pthread_cond_t cond_producer;
extern pthread_cond_t cond_consumer;
extern pthread_t producers[];
extern int producers_amount;

void create_producer(void) {
    if (producers_amount == CHILD_MAX - 1) {
        fputs("Max value of producers\n", stderr);
        return;
    }

    int res = pthread_create(&producers[producers_amount], NULL, produce_handler, NULL);
    if (res) {
        fputs("Failed to create producer\n", stderr);
        exit(res);
    }

    ++producers_amount;

}

_Noreturn void* produce_handler(void* arg){
    msg_t msg;
    int counter;
    while (1) {
        produce(&msg);
        pthread_mutex_lock(&mutex);
        while (queue->message_amount == MSG_MAX)
            pthread_cond_wait(&cond_producer, &mutex);
        counter = add_msg(&msg);
        pthread_cond_signal(&cond_consumer);
        pthread_mutex_unlock(&mutex);

        pthread_t ptid = pthread_self();
        printf(CYAN("%d")"-p) %ld produce msg: hash=%X\n",
               counter, ptid, msg.hash);

        sleep(3);
    }
}

void produce(msg_t *msg) {
    int value = rand() % 257;
    if (value == 256) {
        msg->type = -1;
    } else {
        msg->type = 0;
        msg->size = value;
    }

    for (int i = 0; i < value; ++i) {
        msg->data[i] = (char) (rand() % 256);
    }

    msg->hash = hash(msg);
}


void remove_producer(void) {
    if (producers_amount == 0) {
        fputs("Amount producers = 0\n", stderr);
        return;
    }

    --producers_amount;
    // terminate thread
    pthread_cancel(producers[producers_amount]);
    // wait terminating
    pthread_join(producers[producers_amount], NULL);
}
