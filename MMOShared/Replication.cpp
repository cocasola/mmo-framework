//
// Created by Keegan Beaulieu on 2024-07-17.
//

#include "Component.h"
#include "Entity.h"
#include "Replication.h"

#include <Context.h>
#include <iostream>

#ifdef __SERVER__
void Replication::serialize(unsigned char *&bytes, unsigned char *end, Replication *replication)
{
    Entity *entity = replication->entity;

    serialize_primitive(bytes, end, (uint16_t)entity->instance_id);
    serialize_primitive(bytes, end, (uint16_t)entity->serial_id);
    Vector3f::serialize(bytes, end, &entity->transform->position);
    Vector3f::serialize(bytes, end, &entity->transform->rotation);
    Vector3f::serialize(bytes, end, &entity->transform->scale);

    serialize_primitive(bytes, end, (uint8_t)entity->components.size());

    for (auto ref : entity->components) {
        serialize_primitive(bytes, end, (uint16_t)ref.descriptor->id);

        for (auto expose : ref.descriptor->expose) {
            if (expose.group != Component::Exposure::REPLICATED)
                continue;

            memcpy(bytes, (unsigned char *)ref.instance + expose.offset, expose.size);
            bytes += expose.size;
        }
    }
}
#endif

#ifdef __CLIENT__
void Replication::deserialize(unsigned char *&bytes, unsigned char *end, Replication *replication)
{
    Entity *entity = Entity::create("Replicated Entity", Context::_default);

    deserialize_primitive(bytes, end, (uint16_t &)entity->network_id);
    deserialize_primitive(bytes, end, (uint16_t &)entity->serial_id);
    Vector3f::deserialize(bytes, end, &entity->transform->position);
    Vector3f::deserialize(bytes, end, &entity->transform->rotation);
    Vector3f::deserialize(bytes, end, &entity->transform->scale);

    int components = 0;
    deserialize_primitive(bytes, end, (uint8_t &)components);

    for (int i = 0; i < components; ++i) {
        // ReSharper disable once CppDFAUnreachableCode
        int id = 0;
        deserialize_primitive(bytes, end, (uint16_t &)id);

        Component::Descriptor<Component> *descriptor = Component::descriptor_map[id];

        Component *instance = Component::instance(descriptor, Context::_default);
        instance->entity = entity;

        entity->components.push_back({
            descriptor,
            instance,
            instance->instance_id
        });

        for (auto expose : descriptor->expose) {
            if (expose.group != Component::Exposure::REPLICATED)
                continue;

            memcpy((unsigned char *)instance + expose.offset, bytes, expose.size);
            bytes += expose.size;
        }

        descriptor->on_add(instance);
    }
}
#endif
