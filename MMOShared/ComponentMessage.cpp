//
// Created by Keegan Beaulieu on 2024-07-19.
//

#include "ComponentMessage.h"

void ComponentMessage::serialize(unsigned char *&bytes, unsigned char *end, ComponentMessage *structure)
{
    serialize_primitive(bytes, end, (uint16_t)structure->instance_id);
    serialize_primitive(bytes, end, (uint16_t)structure->message_id);
    memcpy(bytes, structure->message_bytes, structure->message_size);
    bytes += structure->message_size;
}

void ComponentMessage::deserialize(unsigned char *&bytes, unsigned char *end, ComponentMessage *structure)
{
    deserialize_primitive(bytes, end, (uint16_t &)structure->instance_id);
    deserialize_primitive(bytes, end, (uint16_t &)structure->message_id);
    structure->message_bytes = bytes;
    structure->message_size = end - bytes;
}
