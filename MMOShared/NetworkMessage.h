//
// Created by Keegan Beaulieu on 2024-07-13.
//

#ifndef NETWORKMESSAGE_H
#define NETWORKMESSAGE_H

#define BUFFER_SIZE 2048

#include <unordered_map>

#include "Network.h"
#include "Serialization.h"
#include "Session.h"

class NetworkMessage
{
private:
    typedef void(*Callback)(void *data, Session *session);

    struct Handler
    {
        Callback callback;
        Deserializer deserializer;
        size_t data_size;
    };

    static std::unordered_map<int, Handler> handlers;

public:
    static void handle_message(const Packet &packet, Session *session);

    template <typename T>
    static void send_message(const T &data, Session *session);

    template <typename T>
    static void register_handler(void(*callback)(T *, Session *));
};

#include "NetworkTypeMapping.h"

template<typename T>
void NetworkMessage::send_message(const T &data, Session *session)
{
    unsigned char *bytes = (unsigned char *)malloc(BUFFER_SIZE);
    unsigned char *cursor = bytes;

    serialize_primitive(cursor, bytes + BUFFER_SIZE, (uint16_t)NetworkTypeMapping::get_id<T>());

    Serializer serializer = (Serializer)&T::serialize;
    serializer(cursor, bytes + BUFFER_SIZE, (unsigned char *)&data);

    Packet packet{
        (size_t)(cursor - bytes),
        bytes,
        session->endpoint
    };

    Network::send_packet(packet);

    free(bytes);
}

template <typename T>
void NetworkMessage::register_handler(void(*callback)(T *, Session *))
{
    int message_id = NetworkTypeMapping::get_id<T>();

    Handler &handler = handlers[message_id];

    handler.callback = (Callback)callback;
    handler.deserializer = (Deserializer)&T::deserialize;
    handler.data_size = sizeof(T);
}

#endif //NETWORKMESSAGE_H
