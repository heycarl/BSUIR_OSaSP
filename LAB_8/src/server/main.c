#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#include "color_print.h"

#include "cmd/parser.h"
#include "cmd/invoker.h"
#include "cmd/command.h"

#define MAX_CLIENTS 10
#define BUFFER_SIZE 2048

int client_sockets[MAX_CLIENTS];
int num_clients = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *handle_server_msg(void *arg) {
    int client_socket = *((int *) arg);
    free(arg);

    char buffer[BUFFER_SIZE];
    size_t read_size;

    args_t args = {
            client_socket,
            buffer,
            0,
            "."
    };

    while ((read_size = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        printf("New MSG: %s", buffer);
        args.buffer = buffer;
        args.read_bytes = read_size;
        invoke(parse_request(buffer), &args);
        memset(buffer, 0, BUFFER_SIZE);
    }

    close(client_socket);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    uint16_t server_port = strtol(argv[1], NULL, 10);
    server_address.sin_port = htons(server_port);

    if (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 5) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf(YELLOW("Server listening on port %d\n"), ntohs(server_address.sin_port));

    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket < 0) {
            perror("accept");
            continue;
        }

        pthread_t thread_id;
        int *arg = malloc(sizeof(*arg));
        *arg = client_socket;
        pthread_create(&thread_id, NULL, handle_server_msg, arg);

        pthread_mutex_lock(&mutex);
        client_sockets[num_clients++] = client_socket;
        pthread_mutex_unlock(&mutex);

        if (num_clients == MAX_CLIENTS) {
            printf(RED("Maximum number of clients reached. Rejecting new connections.\n"));
            close(client_socket);
        }
    }
    close(server_socket);
    return 0;
}
