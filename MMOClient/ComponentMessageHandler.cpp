//
// Created by Keegan Beaulieu on 2024-07-19.
//

#include "ComponentMessageHandler.h"

#include "Component.h"

void ComponentMessageHandler::handler(ComponentMessage *message, Session *session)
{
    Component::MessageHandler handler = Component::message_handlers[message->message_id];

    void *inner_message = malloc(message->message_size);
    handler.deserializer(message->message_bytes, message->message_bytes + message->message_size, inner_message);

    try {
        Component *component = Component::instance_map.at(message->instance_id);
        handler.callback(component, inner_message);
    } catch (...) {

    }
}
