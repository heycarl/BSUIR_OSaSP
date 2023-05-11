//
// Created by sasha on 5/9/23.
//

#ifndef OSASP_LABS_PROTOCOL_H
#define OSASP_LABS_PROTOCOL_H

#define BUFFER_SIZE 4096
#define PACKET_BUFFER_SIZE (BUFFER_SIZE + 6)

#include <stdint.h>

enum PACKET_TYPE{
    PACKET_INFO,
    PACKET_DIRECTORTY,
    PACKET_ACK,
    PACKET_DISCOVERY_REQ,
    PACKET_DISCOVERY_RESP,
};

typedef struct {
    enum PACKET_TYPE type;
    struct {
        char data[BUFFER_SIZE];
        uint16_t size;
    } payload;
} packet_t;

void set_payload_string(packet_t* packet, char* string) {
    strcpy(packet->payload.data, string);
    packet->payload.size = strlen(packet->payload.data);
}

char* serialize_packet(packet_t* packet) {
    char* buffer = malloc(PACKET_BUFFER_SIZE);
    // Serialize packet type
    uint8_t packet_type = packet->type;
    char *dest = buffer;
    memcpy(buffer, &packet_type, sizeof(packet_type));
    dest += sizeof(packet_type);

    // Serialize payload size
    memcpy(dest, &packet->payload.size, sizeof(packet->payload.size));
    dest += sizeof(packet->payload.size);

    // Serialize payload data
    memcpy(dest, &packet->payload.data, packet->payload.size);
    return buffer;
}

packet_t* deserialize_packet(char* buffer) {
    packet_t* packet = malloc(sizeof(packet_t));

    // Deserialize packet type
    uint8_t packet_type;
    char* src = buffer;
    memcpy(&packet_type, src, sizeof(packet_type));
    packet->type = (enum PACKET_TYPE) packet_type;
    src += sizeof(packet_type);

    // Deserialize payload size
    memcpy(&packet->payload.size, src, sizeof(packet->payload.size));
    src += sizeof(packet->payload.size);

    // Deserialize payload data
    memcpy(&packet->payload.data, src, packet->payload.size);

    return packet;
}

char* packet_type_ui(enum PACKET_TYPE type) {
    switch (type) {
        case PACKET_INFO:
            return "INFO";
        case PACKET_ACK:
            return "ACK";
        case PACKET_DIRECTORTY:
            return "DIR";
        case PACKET_DISCOVERY_REQ:
            return "DISC_REQ";
        case PACKET_DISCOVERY_RESP:
            return "DISC_RESP";
        default:
            return "UNKNOWN";
    }
}

#endif //OSASP_LABS_PROTOCOL_H
