//
// Created by sasha on 5/25/23.
//

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/resource.h>
#include "color_print.h"

typedef struct {
    double time_mark;
    uint64_t recno;
} index_s;

typedef struct {
    uint64_t records;
    index_s idx[10000000];
} index_hdr_s;

void mjd_to_gregorian(double mjd, int *year, int *month, int *day) {
    int a, b, c, d, e;

    if (mjd >= 2400001.0) {
        a = (int)mjd + 2400001 + 68569;
        b = (4 * a) / 146097;
        a = a - (146097 * b + 3) / 4;
    } else {
        a = (int)mjd + 2400001 + 32083;
    }

    c = (4 * a) / 1461;
    d = a - (1461 * c) / 4;
    e = (5 * d + 2) / 153;
    *day = d - (153 * e + 2) / 5 + 1;
    *month = e + 3 - 12 * (e / 10);
    *year = 100 * b + c - 4800 + (e / 10);
}

void generate(char* path, char* filename, size_t size) {
    int file;

    // Generating path to file

    char* absolute_filename = (char*) malloc(PATH_MAX);
    strcat(absolute_filename, filename);
    fprintf(stdout, "%s\n", absolute_filename);

    // Creating file

    if (!(file = open(absolute_filename, O_RDWR, S_IRWXO | S_IRWXU | S_IRWXG))) {
        fprintf(stderr, "Can't create/open file\n");
        exit(-4);
    }

    // Generating structures
    index_hdr_s header;

    // Reading from the file with specific sizeofs
    read(file, &header, sizeof(uint64_t) + size * sizeof(index_s)); // num of records + num of records

    int year, month, day;

    // Printing file to stdout

    fprintf(stdout, YELLOW("RECORDS:") CYAN("%lu\n"), header.records);
    for (size_t i = 0; i < header.records; i++) {
        mjd_to_gregorian(header.idx[i].time_mark, &year, &month, &day);

        fprintf(stdout, YELLOW("Record ID: %lu")"\t"YELLOW("timestamp:") " %f\n", header.idx[i].recno, header.idx[i].time_mark);
//        fprintf(stdout, YELLOW("recno: %lu")"\t"YELLOW("timestamp:") " %d.%d.%d\n", header.idx[i].recno, day, month, year);
    }

    // Closing file and freeing memory

    close(file);
    free(absolute_filename);
}

int main(int argc, char* argv[]) {

    struct rlimit limit;
    getrlimit(RLIMIT_STACK, &limit);
    limit.rlim_cur = limit.rlim_max;
    setrlimit(RLIMIT_STACK, &limit);

    // Check args

    if (argc < 2) {
        fprintf(stderr, RED("[ERROR]")": Not enough arguments, please run program with file name and file size\n");
        return -1;
    }
    long filesize = strtol(argv[2], NULL, 10);
    if (filesize == 0) {
        fprintf(stderr, RED("[ERROR]")": File size is zero or not a number\n");
        return -2;
    }
    if (filesize % 256 != 0) {
        fprintf(stderr, RED("[ERROR]")": File size is not multiple of 256\n");
        return -3;
    }

    generate(argv[0], argv[1], filesize);
    fprintf(stdout, GREEN("Read successfully\n"));
    return 0;
}
