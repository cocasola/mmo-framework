//
// Created by Keegan Beaulieu on 2024-07-19.
//

#ifndef COMPONENTMESSAGE_H
#define COMPONENTMESSAGE_H

#include "Serialization.h"

struct ComponentMessage
{
    int instance_id;
    int message_id;
    unsigned char *message_bytes;
    size_t message_size;

    static void serialize(unsigned char *&bytes, unsigned char *end, ComponentMessage *structure);
    static void deserialize(unsigned char *&bytes, unsigned char *end, ComponentMessage *structure);
};

#endif //COMPONENTMESSAGE_H
