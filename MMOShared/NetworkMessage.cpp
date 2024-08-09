//
// Created by Keegan Beaulieu on 2024-07-13.
//

#define BUFFER_SIZE 2048

#include "NetworkMessage.h"

#include <Serialization.h>

#include "Network.h"

std::unordered_map<int, NetworkMessage::Handler> NetworkMessage::handlers;

void NetworkMessage::handle_message(const Packet &packet, Session *session)
{
    unsigned char *cursor = packet.bytes;

    int message_id = 0;
    deserialize_primitive(cursor, packet.bytes + packet.size, (uint16_t &)message_id);

    try {
        const Handler &handler = handlers.at(message_id);

        unsigned char *structure = (unsigned char *)malloc(handler.data_size);
        handler.deserializer(cursor, packet.bytes + packet.size, structure);
        handler.callback(structure, session);
        free(structure);
    } catch (...) {

    }
}