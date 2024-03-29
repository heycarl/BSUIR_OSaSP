#include <stdio.h>
#include <stdlib.h>

#include <semaphore.h>

#include "init_and_exit.h"
#include "producer.h"
#include "consumer.h"

#include "keyboard_input.h"

static const char options[] = {"Options:\n"
                               "[o] - print options\n"
                               "[P] - create producer\n"
                               "[p] - delete producer\n"
                               "[C] - create consumer\n"
                               "[c] - delete consumer\n"
                               "[q] - quit"};

msg_queue       queue;
pthread_mutex_t mutex;

sem_t* free_space;
sem_t* items;

pthread_t producers[CHILD_MAX];
size_t    producers_amount;

pthread_t consumers[CHILD_MAX];
size_t    consumers_amount;

int main(void) {
    init();

    puts(options);
    fix_keyboard();
    while (1) {
        switch (getchar()) {
            case 'o':
                CLEAR_SCREEN
                puts(options);
                break;
            case 'P':
                create_producer();
                break;
            case 'p':
                remove_producer();
                break;
            case 'C':
                create_consumer();
                break;
            case 'c':
                remove_consumer();
                break;
            case 'q':
                restore_keyboard();
                exit(EXIT_SUCCESS);
            case EOF:
                fputs("Input error", stderr);
                restore_keyboard();
                exit(EXIT_FAILURE);
            default:
                continue;
        }
    }
}
