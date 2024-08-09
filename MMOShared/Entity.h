//
// Created by Keegan Beaulieu on 2024-07-15.
//

#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>
#include <vector>

#include "List.h"
#include "Component.h"
#include "Transform.h"
#include "ReplicationChunk.h"
#include "Event.h"

struct Transform;
class Context;

class Entity
{
public:
    int instance_id;
    int serial_id;

#ifdef __CLIENT__
    int network_id;
#endif

#if defined(__CLIENT__) || defined(__EDITOR__)
    Event<Entity *> on_click;
#endif

    std::string name;
    Transform *transform;
    Context *context;
    Entity **context_entity_ref;
    std::vector<Component::Ref> components;

#ifdef __SERVER__
    ReplicationChunk::Ref replication_chunk_ref;
#endif

    template <typename T>
    T *get_component();

    template <typename T>
    T *add_component();

    static List<Entity> entities;

    static Entity *create(const std::string &name, Context *context);
    static Entity *create(Context *context);
    static void destroy(Entity *entity);
    static void cleanup();
    static void init();

private:
    static int instance_id_counter;
};

#include "NetworkTypeMapping.h"

template<typename T>
T *Entity::get_component()
{
    int id = NetworkTypeMapping::get_id<T>();
    Component::Descriptor<Component> *descriptor = Component::descriptor_map[id];

    for (auto component : components) {
        if (component.descriptor == descriptor)
            return (T *)component.instance;
    }

    return nullptr;
}

template <typename T>
T *Entity::add_component()
{
    int id = NetworkTypeMapping::get_id<T>();

    Component::Descriptor<Component> *descriptor = Component::descriptor_map[id];
    Component *instance = Component::instance(descriptor, context);

    instance->entity = this;
    instance->transform = this->transform;
    components.push_back({ descriptor, instance });

    descriptor->on_add(instance);

    return (T *)instance;
}

#endif //ENTITY_H
