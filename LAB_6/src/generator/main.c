//
// Created by sasha on 4/25/23.
//

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

struct index_s {
    double time_mark; // timestamp
    uint64_t recno;   // record INDEX
};

struct index_hdr_s {
    uint64_t records;     // records num in DB
    struct index_s idx[]; // records array
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <index_size>\n", argv[0]);
        return 1;
    }

    char* filename = argv[1];
//    uint64_t records = atoi(argv[2]);
    char **end = NULL;
    uint64_t records = strtol(argv[2], end, 10);

    if (records % 256 != 0) {
        fprintf(stderr,"Index %% 256 should be 0\n");
        return 1;
    }

    struct index_hdr_s* header = (struct index_hdr_s*)malloc(sizeof(struct index_hdr_s) + records * sizeof(struct index_s));
    if (header == NULL) {
        fprintf(stderr,"Memory allocation error\n");
        return 1;
    }

    header->records = records;

    srand(time(0));

    for (uint64_t i = 0; i < records; i++) {
        double time_mark = 15020.0 + (rand() / (double)RAND_MAX) * (365.0 - 0.5);
        uint64_t recno = 1 + rand() % (UINT64_MAX - 1);
        header->idx[i].time_mark = time_mark;
        header->idx[i].recno = recno;
    }

    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        fprintf(stderr, "File opening error\n");
        free(header);
        return 1;
    }

    fwrite(header, sizeof(struct index_hdr_s) + records * sizeof(struct index_s), 1, file);
    fclose(file);

    free(header);
    fprintf(stdout, "File generated: %s\n", filename);

    return 0;
}
