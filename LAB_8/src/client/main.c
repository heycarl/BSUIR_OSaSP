#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdlib.h>

#include "color_print.h"

#define BUFFER_SIZE 1024

void *handle_server_msg(void *arg) {
    char buffer[BUFFER_SIZE];
    int* client_socket = arg;
    while (1) {
        ssize_t bytes_received = recv(*client_socket, buffer, BUFFER_SIZE-1, 0);
        if (bytes_received == -1) {
            perror("Failed to receive response");
            break;
        }

        buffer[bytes_received] = 0; // add null terminator
        printf(YELLOW("Server response: ") "%s\n", buffer);
        if (strcmp(buffer, "Bye!\n") == 0)
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
        ssize_t bytes_sent = send(client_socket, buffer, strlen(buffer), 0);
        if (bytes_sent == -1) {
            perror("Failed to send command");
            break;
        }
        memset(buffer, 0, BUFFER_SIZE);
    }

    close(client_socket);
    return 0;
}
