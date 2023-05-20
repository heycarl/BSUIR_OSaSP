//
// Created by sasha on 5/17/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <pthread.h>

#include "message/message.h"
#include "producer/producer.h"
#include "consumer/consumer.h"

#include "color_print.h"

void atexit_handler(void);
void initialization(void);
void info(void);

const char *menu = CYAN("MENU:\n")
                   CYAN("[1]")" - print options\n"
                   CYAN("[2]")" - create producer\n"
                   CYAN("[3]")" - delete producer\n"
                   CYAN("[4]")" - create consumer\n"
                   CYAN("[5]")" - delete consumer\n"
                   CYAN("[6]")" - info\n"
                   CYAN("[+]")" - increase queue\n"
                   CYAN("[-]")" - decrease queue\n"
                   CYAN("[7]")" - quit";

msg_struct* queue; // ring buffer of messages

pthread_mutex_t mutex;


pthread_cond_t cond_producer = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_consumer = PTHREAD_COND_INITIALIZER;


// Processes info
pthread_t producers[CHILD_MAX];
int producers_amount;
pthread_t consumers[CHILD_MAX];
int consumers_amount;

msg_t msg;
int counter;
int main(void) {
    initialization();

    puts(menu);
    while (1) {
        switch (getchar()) {
            case '1':
                puts(menu);
                break;

            case '2':
                create_producer();
                break;

            case '3':
                remove_producer();
                break;

            case '4':
                create_consumer();
                break;

            case '5':
                remove_consumer();
                break;

            case '6':
                info();
                break;

            case '+':
                produce(&msg);
                pthread_mutex_lock(&mutex);
                while (queue->message_amount == MSG_MAX)
                    pthread_cond_wait(&cond_producer, &mutex);
                counter = add_msg(&msg);
                pthread_cond_signal(&cond_consumer);
                pthread_mutex_unlock(&mutex);

                printf("%d) User produce msg: hash=%X\n",
                       counter, msg.hash);

                break;

            case '-':
                if(queue->message_amount == 0){
                    printf(RED("There are no messages in the queue\n"));
                    break;
                }
                pthread_mutex_lock(&mutex);
                while (queue->message_amount == 0)
                    pthread_cond_wait(&cond_consumer, &mutex);

                counter = get_msg(&msg);

                pthread_cond_signal(&cond_producer);
                pthread_mutex_unlock(&mutex);

                consume(&msg);

                printf("%d) User consume msg: hash=%X\n",
                       counter, msg.hash);
                break;

            case '7':
                exit(EXIT_SUCCESS);

            case EOF:
                fputs(RED("Input error"), stderr);
                exit(EXIT_FAILURE);

            default:
                break;
        }
    }
    return 0;
}


void initialization(void) {
    atexit(atexit_handler);

    // Setup shared memory
    int file_descriptor = shm_open(SHARED_MEMORY_OBJ, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (file_descriptor < 0) {
        perror("shm_open");
        exit(errno);
    }

    // set length of file
    if (ftruncate(file_descriptor, sizeof(msg_struct))) {
        perror("ftruncate");
        exit(errno);
    }

    // copy bytes from file description into memory
    // PROT_* set rules for this part of memory
    // MAP_SHARED - flags set allowing for other processes to use
    // return pointer to memory with shared data
    void *ptr = mmap(NULL, sizeof(msg_struct), PROT_READ | PROT_WRITE,
                     MAP_SHARED, file_descriptor, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(errno);
    }

    queue = (msg_struct*) ptr;

    // Setup queue
    init_queue();


    // Setup mutex
    int res = pthread_mutex_init(&mutex, NULL);
    if(res){
        fputs(RED("Failed mutex init\n"), stderr);
        exit(res);
    }

    pthread_cond_init(&cond_producer, NULL);
    pthread_cond_init(&cond_consumer, NULL);

    if (close(file_descriptor)) {
        perror("close");
        exit(errno);
    }
}

void atexit_handler(void) {
    for(int i = 0; i < producers_amount; i++) remove_producer();
    for(int i = 0; i < consumers_amount; i++) remove_consumer();

    // remove named semaphore
    pthread_cond_destroy(&cond_producer);
    pthread_cond_destroy(&cond_consumer);

    pthread_mutex_unlock(&mutex);
    pthread_mutex_destroy(&mutex);

    // remove shared memory segment
    if (shm_unlink(SHARED_MEMORY_OBJ)) {
        perror("shm_unlink");
        abort();
    }

}


void info(void){
    pthread_mutex_lock(&mutex);
    printf("Count producer: %d; count consumer: %d; count messages in queue: %d\n", producers_amount, consumers_amount, queue->message_amount);
    pthread_mutex_unlock(&mutex);
}
