//
// Created by sasha on 5/11/23.
//

#ifndef OSASP_LABS_THREAD_H
#define OSASP_LABS_THREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "utils.h"

void createThreads(long amount, long memsize, long blocks, char* path, char* filename);
void joinThreads(int amount);

#endif //OSASP_LABS_THREAD_H
