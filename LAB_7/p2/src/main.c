//
// Created by sasha on 5/17/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "color_print.h"

#define RECORD_SIZE 168
#define NUM_RECORDS 15

const char *menu =  CYAN("Commands:\n")
                    CYAN("ADD")" - add a new record\n"
                    CYAN("LST")" - list all records\n"
                    CYAN("GET")" <record number> - get record with number n\n"
                    CYAN("MOD")" - modify an existing record\n"
                    CYAN("PUT")" - save modified record\n"
                    CYAN("QUIT")" - exit the program\n";

struct record_s {
    char name[80];
    char address[80];
    uint8_t semester;
};
/**
 * Get lock state
 * @param fd File Descriptor
 * @param offset Memory offset
 * @param size Lock size
 * @return Is data locked
 */
bool get_lock_state(int fd, off_t offset, off_t size) {
    struct flock lock;
    lock.l_type = F_WRLCK;  // Check for a Write lock
    lock.l_whence = SEEK_SET;
    lock.l_start = offset;
    lock.l_len = size;

    if (fcntl(fd, F_GETLK, &lock) == -1) {
        perror("Failed to get lock information");
        return false;
    }

    return lock.l_type == F_WRLCK;
}
/**
 * Lock
 * @param fd File Descriptor
 * @param offset Memory offset
 * @param size Lock size
 * @param lock_type Lock type
 * @return Lock result
 */
int lock(int fd, off_t offset, off_t size, short lock_type) {
    struct flock lock;
    lock.l_type = lock_type;
    lock.l_whence = SEEK_SET;
    lock.l_start = offset;
    lock.l_len = size;

    return fcntl(fd, F_SETLK, &lock);
}
/**
 * Unlock
 * @param fd File Descriptor
 * @param offset Memory offset
 * @param size Unlock size
 * @return Unlock result code
 */
int unlock(int fd, off_t offset, off_t size) {
    struct flock lock;
    lock.l_type = F_UNLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = offset;
    lock.l_len = size;

    return fcntl(fd, F_SETLK, &lock);
}

int get_record(int fd, int record_num, struct record_s *record) {
    if(lseek(fd, record_num * RECORD_SIZE, SEEK_SET) == -1){
        return -1;
    }
    if(read(fd, record, sizeof(struct record_s)) <= 0){
        return -1;
    }
    return 0;
}

void list_records(int fd) {
    struct record_s record;
    printf("Records:\n");
    for (int i = 0; i < NUM_RECORDS; i++) {
        if(!get_record(fd, i, &record))
            printf(BLUE("[%d]")" %s, %s, %d\n", i, record.name, record.address, record.semester);
        else return;
    }
}

void put_record(int fd, int record_num, struct record_s *record) {
    if(lseek(fd, record_num * RECORD_SIZE, SEEK_SET) == -1){
        perror("lseek");
        return;
    }
    sleep(10);
    if(write(fd, record, sizeof(struct record_s)) == -1){
        perror("write");
        return;
    }
}

void modify_record(struct record_s *record) {
    char input[80];
    printf(YELLOW("Enter name: "));
    if(!fgets(input, 80, stdin)) printf(RED("Error: fgets\n"));
    input[strcspn(input, "\n")] = 0;
    strncpy(record->name, input, 80);

    printf(YELLOW("Enter address: "));
    if(!fgets(input, 80, stdin)) printf(RED("Error: fgets\n"));
    input[strcspn(input, "\n")] = 0;
    strncpy(record->address, input, 80);

    printf(YELLOW("Enter semester: "));
    if(!fgets(input, 80, stdin)) printf(RED("Error: fgets\n"));
    input[strcspn(input, "\n")] = 0;
    record->semester = strtol(input, NULL, 10);
}

void print_record(struct record_s record, int record_num) {
    printf(CYAN("Record:")" #%d\n", record_num);
    printf(CYAN("Name:")" %s\n", record.name);
    printf(CYAN("Address:")" %s\n", record.address);
    printf(CYAN("Semester:")" %d\n", record.semester);
    printf("\n");
}

bool cmp_record(struct record_s record1, struct record_s record2){
    return !strcmp(record1.name, record2.name) && !strcmp(record1.address, record2.address) && record1.semester == record2.semester;
}

int main(void) {
    int fd = open("records.dat", O_RDWR | O_CREAT,  S_IRWXU | S_IRWXG |  S_IRWXO);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    printf("%s", menu);

    char input[80];
    int record_num = -1;
    struct record_s record;
    while (1) {
        printf("> ");
        if(!fgets(input, 80, stdin)) printf("Error: fgets\n");
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "LST") == 0) {
            list_records(fd);
        } else if (strcmp(input, "ADD") == 0) {
            record_num = -1;
            for (int i = 0; i < NUM_RECORDS; i++) {
                if (get_record(fd, i, &record)) {
                    record_num = i;
                    break;
                }
            }
            if (record_num == -1) {
                printf(RED("No free records\n"));
            } else if (get_lock_state(fd, record_num * RECORD_SIZE, RECORD_SIZE)) {
                printf(RED("Record is locked\n"));
            } else {
                lock(fd, record_num * RECORD_SIZE, RECORD_SIZE, F_WRLCK);
                memset(&record, 0, sizeof(struct record_s));
                modify_record(&record);
                put_record(fd, record_num, &record);
                unlock(fd,record_num * RECORD_SIZE, RECORD_SIZE);
                printf(CYAN("Record added at position %d\n"), record_num);
            }
        } else if (strncmp(input, "MOD", 3) == 0) {
            if(record_num == -1){
                printf(CYAN("Nothing to edit. Call GET function!\n"));
            } else {
                struct record_s record_sav;
                AGAIN:
                record_sav = record;
                modify_record(&record);
                if(!cmp_record(record, record_sav)){
                    if (get_lock_state(fd, record_num * RECORD_SIZE, RECORD_SIZE)) {
                        printf(RED("Record is locked\n"));
                    } else {
                        lock(fd, record_num * RECORD_SIZE, RECORD_SIZE, F_WRLCK);
                        struct record_s record_new;
                        get_record(fd, record_num, &record_new);
                        if(cmp_record(record_sav, record_new)){
                            // одинаковые (никто не изменил пока мы думали)
                            // put_record(fd, record_num, &record);
                            unlock(fd,record_num * RECORD_SIZE, RECORD_SIZE);
                            printf(CYAN("Record modified at position %d\n"), record_num);
                        } else {
                            // кто-то изменил запись после получения ее нами
                            // повторяем с ее новым содержимым
                            printf(CYAN("Data has been updated, please try again!\n"));
                            unlock(fd,record_num * RECORD_SIZE, RECORD_SIZE);
                            record = record_new;
                            goto AGAIN;
                        }
                    }
                }
            }
        } else if (strncmp(input, "GET ", 4) == 0) {
            record_num = (int) strtol(input + 4, NULL, 10);
            get_record(fd, record_num, &record);
            print_record(record, record_num);
        } else if (strcmp(input, "PUT") == 0) {
            if (record_num == -1) {
                printf("No record to save\n");
            } else if (get_lock_state(fd, record_num * RECORD_SIZE, RECORD_SIZE)) {
                printf(RED("Record is locked\n"));
            } else {
                lock(fd, record_num * RECORD_SIZE, RECORD_SIZE, F_WRLCK);
                put_record(fd, record_num, &record);
                unlock(fd,record_num * RECORD_SIZE, RECORD_SIZE);
                printf("Record #%d saved\n", record_num);
            }
        } else if (strcmp(input, "QUIT") == 0) {
            break;
        } else {
            printf("Unknown command\n");
        }
    }

    close(fd);
    return 0;
}
