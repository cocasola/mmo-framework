//
// Created by Keegan Beaulieu on 2024-07-15.
//

#include "Component.h"
#include "Entity.h"
#include "Context.h"

#include <iostream>
#include <UiViewport.h>

List<Entity> Entity::entities;
int Entity::instance_id_counter = 1;

Entity *Entity::create(const std::string &name, Context *context)
{
    if (!context)
        context = Context::_default;

    Entity *entity = entities.alloc();
    entity->name = name;
    entity->transform = Transform::create();
    entity->instance_id = instance_id_counter++;
    entity->context = context;
    entity->context_entity_ref = context->entities.alloc(entity);

    return entity;
}

Entity *Entity::create(Context *context)
{
    return create("Entity", context);
}

void Entity::destroy(Entity *entity)
{
    for (auto ref : entity->components) {
        ref.descriptor->on_remove((Component *)ref.instance);
        ref.descriptor->context_instances[entity->context].destroy(ref.instance->context_instance_ref);
        ref.descriptor->instances.destroy((Component *)ref.instance);
    }

    entity->context->entities.destroy(entity->context_entity_ref);
    Transform::destroy(entity->transform);
    entities.destroy(entity);
}

void Entity::cleanup()
{
    for (auto i : entities) {
        destroy(&i);
    }
}

void Entity::init()
{

}
