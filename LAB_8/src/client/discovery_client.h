//
// Created by sasha on 5/10/23.
//

#ifndef OSASP_LABS_DISCOVERY_CLIENT_H
#define OSASP_LABS_DISCOVERY_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "protocol.h"
#include "color_print.h"

// multicast ip range 224.0.0.0 to 239.255.255.255
#define MULTICAST_GROUP "230.10.10.1"
#define PORT 5000

pthread_barrier_t server_discovery_barrier;

void *multicast_thread_handler(void *arg) {
    char* server_address_char = arg;
    struct sockaddr_in dest_addr = {0};
    ssize_t num_bytes;

    // Create a socket for sending
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Allow multiple sockets to use the same port number
    int reuse_addr = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to the local address and port
    struct sockaddr_in local_addr = {0};
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = htons(PORT);
    if (bind(sock, (struct sockaddr*) &local_addr, sizeof(local_addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Join the multicast group
    struct ip_mreq multicast_request = {0};
    multicast_request.imr_multiaddr.s_addr = inet_addr(MULTICAST_GROUP);
    multicast_request.imr_interface.s_addr = INADDR_ANY;
    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multicast_request, sizeof(multicast_request)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Set up the address for sending to the multicast group
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr.s_addr = inet_addr(MULTICAST_GROUP);
    dest_addr.sin_port = htons(PORT);

    // Send the request to the multicast group
    packet_t server_discovery_req;
    server_discovery_req.type = PACKET_DISCOVERY_REQ;
    char hostname[HOST_NAME_MAX];
    gethostname(hostname, HOST_NAME_MAX);
    set_payload_string(&server_discovery_req, hostname);
    num_bytes = sendto(sock, serialize_packet(&server_discovery_req), PACKET_BUFFER_SIZE, 0,
                       (struct sockaddr *)&dest_addr, sizeof(dest_addr));

    if (num_bytes == -1) {
        perror("sendto");
        exit(EXIT_FAILURE);
    }
    printf(CYAN("Sent discovery request to %s:%d\n"), MULTICAST_GROUP, PORT);

    // Receive packets from the multicast group
    struct sockaddr_in src_addr = {0};
    socklen_t addrlen = sizeof(src_addr);
    ssize_t nbytes;

    char buffer[BUFFER_SIZE];
    while ((recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr*) &src_addr, &addrlen)) > 0) {
        packet_t* packet = deserialize_packet(buffer);
        printf(YELLOW("Multicast packet recived [%s] : ") "%s\n", packet_type_ui(packet->type), packet->payload.data);
        if (packet->type == PACKET_DISCOVERY_RESP) {
            strcpy(server_address_char, packet->payload.data);
            break;
        }
    }


    pthread_barrier_wait(&server_discovery_barrier);

    // Leave the multicast group and close the socket
    setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, &multicast_request, sizeof(multicast_request));
    close(sock);
    return NULL;
}

#endif //OSASP_LABS_DISCOVERY_CLIENT_H
