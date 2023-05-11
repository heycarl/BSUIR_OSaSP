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

// multicast ip range 224.0.0.0 to 239.255.255.255
#define MULTICAST_GROUP "230.10.10.1"
#define PORT 5000


void *multicast_thread_handler(void *arg) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
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

    // Receive packets from the multicast group
    char buffer[BUFFER_SIZE];
    struct sockaddr_in src_addr = {0};
    socklen_t addrlen = sizeof(src_addr);
    ssize_t nbytes;

    char src_ip_char[INET_ADDRSTRLEN];

    struct sockaddr_in dest_addr = {0};
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr.s_addr = inet_addr(MULTICAST_GROUP);
    dest_addr.sin_port = htons(PORT);

    while ((recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr*) &src_addr, &addrlen)) > 0) {

        packet_t* packet = deserialize_packet(buffer);
        inet_ntop(AF_INET, &src_addr.sin_addr, src_ip_char, INET_ADDRSTRLEN);
        printf(YELLOW("Multicast packet recived ") CYAN("[%s]") YELLOW(" : ") "%s\n", packet_type_ui(packet->type), packet->payload.data);
        if (packet->type == PACKET_DISCOVERY_REQ) {
            packet_t server_discovery_resp;
            server_discovery_resp.type = PACKET_DISCOVERY_RESP;
            set_payload_string(&server_discovery_resp, src_ip_char);
            sendto(sock, serialize_packet(&server_discovery_resp), PACKET_BUFFER_SIZE,
                   0, (struct sockaddr*) &dest_addr, sizeof(dest_addr));
        }
    }

    // Leave the multicast group and close the socket
    setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, &multicast_request, sizeof(multicast_request));
    close(sock);

    return NULL;
}
#endif //OSASP_LABS_DISCOVERY_CLIENT_H
