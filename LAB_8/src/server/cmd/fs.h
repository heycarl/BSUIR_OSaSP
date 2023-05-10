//
// Created by sasha on 5/7/23.
//

#ifndef OSASP_LABS_FS_H
#define OSASP_LABS_FS_H

#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdbool.h>

#include "command.h"

void process_path(char* current_dir, char* wanted_dir) {
    char* striped_path = wanted_dir + strlen("CD ") * sizeof(char);
    strcat(current_dir, "/");
    strcat(current_dir, striped_path);
    realpath(current_dir, current_dir);
}

void cmd_cd(args_t* args) {
    process_path(args->current_path, args->buffer);
    DIR *dir = NULL;
    dir = opendir(args->current_path);
    if (dir == NULL ) {
        printf( "Error opening %s: %s", args->current_path, strerror(errno));
        return;
    }
    packet_t response;
    response.type = PACKET_DIRECTORTY;
    set_payload_string(&response, args->current_path);
    send(args->socket, serialize_packet(&response), PACKET_BUFFER_SIZE, 0);
}

void cmd_list(args_t* args) {
    struct dirent *entry = NULL;
    DIR *dir = NULL;
    dir = opendir(args->current_path);
    if (dir == NULL ) {
        printf( "Error opening %s: %s", args->current_path, strerror(errno));
        return;
    }
    entry = readdir(dir);

    char pathName[PATH_MAX + 1];
    packet_t response;
    char response_buffer[128];
    while(entry != NULL) {
        if((strncmp(entry->d_name, ".", PATH_MAX) == 0) || (strncmp(entry->d_name, "..", PATH_MAX) == 0)) {
            entry = readdir(dir);
            continue;
        }
        strncpy(pathName, args->current_path, PATH_MAX);
        strncat(pathName, "/", PATH_MAX);
        strncat(pathName, entry->d_name, PATH_MAX);

        uint16_t path_offset = 0;
//        uint16_t path_offset = strlen(pathName);

        struct stat entryInfo;
        if(lstat(pathName, &entryInfo) == 0) {
            if(S_ISDIR(entryInfo.st_mode)) {
                sprintf(response_buffer,"%s/\n", pathName + path_offset);
            } else if(S_ISREG(entryInfo.st_mode)) {
                sprintf(response_buffer, "%s \n", pathName + path_offset);
            } else if(S_ISLNK(entryInfo.st_mode)) {
                char targetName[PATH_MAX + 1];
                if(readlink(pathName, targetName, PATH_MAX) != -1) {
                    sprintf(response_buffer, "%s -> %s\n", pathName + path_offset, targetName);
                }
            }
        } else {
            sprintf(response_buffer, "Error starting %s: %s\n", pathName, strerror(errno));
        }
        strcat(response.payload.data, response_buffer);
        response.payload.size += strlen(response_buffer);
        memset(response_buffer, 0 , sizeof(response_buffer));
        entry = readdir(dir);
    }
    send(args->socket, serialize_packet(&response), PACKET_BUFFER_SIZE, 0);
    memset(response.payload.data, 0, BUFFER_SIZE);
    closedir(dir);
}

#endif //OSASP_LABS_FS_H
