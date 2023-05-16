//
// Created by sasha on 5/11/23.
//

#ifndef OSASP_LABS_UTILS_H
#define OSASP_LABS_UTILS_H

#include <stdint.h>
#include <stddef.h>
#include <pthread.h>

typedef struct {
    double time_mark;
    uint64_t recno;
} index_s;

typedef struct {
    uint64_t records;
    index_s idx[1000000000];
} index_hdr_s;

typedef struct {
    char* addr;
    long memsize;
    long blocks;
    size_t no;
} args;

typedef struct {
    int isBusy;
    int block;
} blocks_map;

#endif //OSASP_LABS_UTILS_H
