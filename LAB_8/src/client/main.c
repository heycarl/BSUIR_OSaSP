#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdlib.h>
#include <limits.h>

#include "protocol.h"

#include "color_print.h"

pthread_barrier_t response_wait_barrier;

//#define MAX_SCRIPT_CMD 25
//#define MAX_CMD_LENGTH 50
//
//char current_dir[PATH_MAX];
//
//
//typedef struct {
//  uint8_t cmd_num;
//  char cmd[MAX_SCRIPT_CMD][BUFFER_SIZE];
//} cmd_list;
//
//void parse_script_file(cmd_list* list, char* filename) {
//    char script_path[PATH_MAX];
//    strcat(script_path, "./");
//    strcat(script_path, filename + sizeof(char));
//    FILE* script_file = fopen(script_path, "rt");
//    uint8_t cmd_index = 0;
//    while (fgets(list->cmd[list->cmd_num], MAX_CMD_LENGTH, script_file)) {
//        list->cmd[list->cmd_num][strlen(list->cmd[list->cmd_num])-1] = '\0';
//        list->cmd_num++;
//    }
//}

void *handle_server_msg(void *arg) {
    char buffer[PACKET_BUFFER_SIZE];
    int *client_socket = arg;
    while (1) {
        ssize_t bytes_received = recv(*client_socket, buffer, PACKET_BUFFER_SIZE, 0);
        if (bytes_received == -1) {
            perror("Failed to receive response");
            break;
        }

        packet_t* packet = deserialize_packet(buffer);
        switch (packet->type) {
            case PACKET_INFO:
                printf(YELLOW("Server response : ") "%s\n", packet->payload.data);
                printf(YELLOW("%s> "), current_dir);
                break;
            case PACKET_DIRECTORTY:
                strcpy(current_dir, packet->payload.data);
                printf(YELLOW("%s> "), current_dir);
                break;
            case PACKET_ACK:
                printf(CYAN("ACK response : ") "%s\n", packet->payload.data);
                break;
            default:
                continue;
        }
        pthread_barrier_wait(&response_wait_barrier);

        if (strcmp(buffer, "Bye!") == 0)
            break;
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <ip-address>\n", argv[0]);
        return 1;
    }

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Failed to create socket");
        return 1;
    }

    pthread_barrier_init(&response_wait_barrier, NULL, 2);

    struct sockaddr_in server_address = {0};
    server_address.sin_family = AF_INET;
    uint16_t server_port = strtol(argv[2], NULL, 10);
    server_address.sin_port = htons(server_port);
    if (inet_pton(AF_INET, argv[1], &server_address.sin_addr) == -1) {
        perror("Invalid IP address");
        return 1;
    }

    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Failed to connect to server");
        return 1;
    }

    char buffer[BUFFER_SIZE];
    pthread_t rx_thread;
    pthread_create(&rx_thread, NULL, handle_server_msg, &client_socket);
    printf(YELLOW("> "));
    while (1) {
        fgets(buffer, BUFFER_SIZE, stdin);
        if (strlen(buffer) == 0) {
            continue; // ignore empty commands
        }
        buffer[strlen(buffer)-1] = '\0';
//        if (strstr(buffer, "@") != NULL) { // script file
//            cmd_list script_cmds;
//            parse_script_file(&script_cmds, buffer);
//            for (int i = 0; i < script_cmds.cmd_num; ++i) {
//                send(client_socket, script_cmds.cmd[i], strlen(script_cmds.cmd[i]), 0);
//            }
//            continue;
//        }
        ssize_t bytes_sent = send(client_socket, buffer, strlen(buffer), 0);
        pthread_barrier_wait(&response_wait_barrier);
        if (strcmp(buffer, "QUIT") == 0)
            break;
        if (bytes_sent == -1) {
            perror("Failed to send command");
            break;
        }
        memset(buffer, 0, BUFFER_SIZE);
    }

    close(client_socket);
    return 0;
}
