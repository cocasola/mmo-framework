//
// Created by Keegan Beaulieu on 2024-07-16.
//

#include "Component.h"

List<Component::Descriptor<Component>> Component::descriptors;
std::unordered_map<int, Component::Descriptor<Component> *> Component::descriptor_map;
int Component::instance_id_counter = 1;
std::unordered_map<int, Component::MessageHandler> Component::message_handlers;
std::unordered_map<int, Component *> Component::instance_map;
List<Component::TickOrderCallbacks> Component::tick_order_callbacks;
std::unordered_map<const std::type_info *, int> Component::exposure_type_id_map;
int Component::exposure_type_id_counter = 1;

Component *Component::instance(Descriptor<Component> *descriptor, Context *context)
{
    Component *instance = (Component *)descriptor->instances.alloc(descriptor->size);
    instance->instance_id = instance_id_counter++;
    instance->context_instance_ref = descriptor->context_instances[context].alloc(instance);
    instance_map[instance->instance_id] = instance;

    return instance;
}

void Component::tick_components()
{
    for (auto tick_order_callback : tick_order_callbacks) {
        for (auto callback : tick_order_callback.callbacks) {
            callback.callback(callback.descriptor);
        }
    }
}
