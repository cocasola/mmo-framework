//
// Created by Keegan Beaulieu on 2024-07-16.
//

#ifndef COMPONENT_H
#define COMPONENT_H

#include <unordered_map>

#include "List.h"
#include "Serialization.h"
#include "ComponentMessage.h"
#include "Vector2i.h"
#include "ReplicationChunk.h"
#include "Transform.h"

class Entity;
class Context;

class Component
{
public:
    struct Exposure
    {
        enum Group
        {
            SERVER,
            CLIENT,
            REPLICATED
        };

        Group group;
        size_t offset;
        size_t size;
        std::string name;
        const std::type_info *type;
        int type_id;
    };

    enum TickOrder
    {
        SIMULATION = 1,
        PHYSICS,
        RENDERING
    };

    template <typename T>
    struct Descriptor;

    template <typename T>
    struct TickCallback
    {
        void(*callback)(Descriptor<T> *);
        TickOrder order;
        bool execute_in_editor;
    };

    template <typename T>
    struct Descriptor
    {
        List<T> instances;
        std::unordered_map<Context *, List<T *>> context_instances;
        int id;
        void(*on_add)(T *);
        void(*on_remove)(T *);
        std::vector<TickCallback<T>> tick_callbacks;
        size_t size;
        std::vector<Exposure> expose;
        std::string name;
    };

    struct Ref
    {
        Descriptor<Component> *descriptor;
        Component *instance;

#ifdef __SERVER__
        bool replicate;
#endif
    };

    template <typename T>
    struct Params
    {
        void(*on_add)(T *) = nullptr;
        void(*on_remove)(T *) = nullptr;
        std::vector<TickCallback<T>> tick_callbacks;
        std::vector<Exposure> expose;
        std::string name = "Anonymous Component";
    };

    typedef void(*MessageCallback)(Component *, void *);
    typedef void(*TickCallbackFn)(Descriptor<Component> *);

    struct MessageHandler
    {
        MessageCallback callback;
        Deserializer deserializer;
        size_t size;
    };

    struct TickOrderCallback
    {
        TickCallbackFn callback;
        Descriptor<Component> *descriptor;
    };

    struct TickOrderCallbacks
    {
        TickOrder order;
        List<TickOrderCallback> callbacks;
    };

    int instance_id;
    Entity *entity;
    Transform *transform;
    Component **context_instance_ref;

    template <typename T>
    void send_message(const T &message);

    static List<Descriptor<Component>> descriptors;
    static std::unordered_map<int, Descriptor<Component> *> descriptor_map;

    template <typename T>
    static Descriptor<T> *register_type(const Params<T> &params);

    template <typename T>
    static Descriptor<T> *get_descriptor();

    template <typename C, typename T>
    static void register_message_handler(void(*handler)(C *component, T *message));

    static Component *instance(Descriptor<Component> *descriptor, Context *context);
    static void tick_components();

    static int instance_id_counter;
    static int exposure_type_id_counter;
    static std::unordered_map<int, MessageHandler> message_handlers;
    static std::unordered_map<int, Component *> instance_map;
    static std::unordered_map<const std::type_info *, int> exposure_type_id_map;
    static List<TickOrderCallbacks> tick_order_callbacks;
};

#include "Entity.h"
#include "NetworkMessage.h"
#include "NetworkTypeMapping.h"

template<typename T>
void Component::send_message(const T &message)
{
    unsigned char *message_bytes = (unsigned char *)malloc(BUFFER_SIZE);
    unsigned char *cursor = message_bytes;

    T::serialize(cursor, message_bytes + BUFFER_SIZE, &message);

    ComponentMessage full_message = {
        instance_id,
        NetworkTypeMapping::get_id<T>(),
        message_bytes,
        (size_t)(cursor - message_bytes)
    };

    Vector2i coords = ReplicationChunk::get_chunk_coords(entity->transform->position);

    for (int x = coords.x - 1; x < coords.x + 1; ++x) {
        for (int y = coords.y - 1; y < coords.y + 1; ++y) {
            ReplicationChunk *chunk = ReplicationChunk::get_chunk(Vector2i(x, y));

            for (auto session : chunk->sessions) {
                NetworkMessage::send_message(full_message, (Session *)session);
            }
        }
    }

    free(message_bytes);
}

template <typename T>
Component::Descriptor<T> *Component::register_type(const Params<T> &params)
{
    void(*on_add)(T *) = params.on_add ? params.on_add : [](T *) -> void { };
    void(*on_remove)(T *) = params.on_remove ? params.on_remove : [](T *) -> void { };

    int id = NetworkTypeMapping::get_id<T>();

    Descriptor<T> *descriptor = (Descriptor<T> *)descriptors.alloc();
    descriptor_map[id] = (Descriptor<Component> *)descriptor;

    descriptor->name = params.name;
    descriptor->context_instances = std::unordered_map<Context *, List<T *>>();
    descriptor->id = id;
    descriptor->on_add = on_add;
    descriptor->on_remove = on_remove;
    descriptor->size = sizeof(T);
    descriptor->expose = params.expose;

    for (auto tick_callback : params.tick_callbacks) {
#ifdef __EDITOR__
        if (!tick_callback.execute_in_editor)
            continue;
#endif

        bool matched_order = false;

        for (auto tick_order_callback : tick_order_callbacks) {
            if (tick_callback.order < tick_order_callback.order) {
                TickOrderCallbacks *new_callbacks = tick_order_callbacks.alloc_before(&tick_order_callback);
                new_callbacks->order = tick_callback.order;
                new_callbacks->callbacks.alloc({
                    .callback = (TickCallbackFn)tick_callback.callback,
                    .descriptor = (Descriptor<Component> *)descriptor
                });

                matched_order = true;
                break;
            } else if (tick_callback.order == tick_order_callback.order) {
                tick_order_callback.callbacks.alloc({
                    .callback = (TickCallbackFn)tick_callback.callback,
                    .descriptor = (Descriptor<Component> *)descriptor
                });

                matched_order = true;
                break;
            }
        }

        if (!matched_order) {
            TickOrderCallbacks *new_callbacks = tick_order_callbacks.alloc();
            new_callbacks->order = tick_callback.order;
            new_callbacks->callbacks.alloc({
                .callback = (TickCallbackFn)tick_callback.callback,
                .descriptor = (Descriptor<Component> *)descriptor
            });
        }
    }

    return descriptor;
}

template<typename T>
Component::Descriptor<T> *Component::get_descriptor()
{
    int id = NetworkTypeMapping::get_id<T>();
    return (Descriptor<T> *)descriptor_map[id];
}

template<typename C, typename T>
void Component::register_message_handler(void(*callback)(C *component, T *message))
{
    int id = NetworkTypeMapping::get_id<T>();

    message_handlers[id] = {
        (MessageCallback)callback,
        (Deserializer)&T::deserialize,
        sizeof(T)
    };
}

#define EXPOSE(group, type, member) (Exposure){\
    Component::Exposure::Group::group,\
    offsetof(type, member),\
    sizeof(type().member),\
    #member,\
    &typeid(decltype(((type *)0)->member))\
},

#endif //COMPONENT_H
