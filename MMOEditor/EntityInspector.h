//
// Created by Keegan Beaulieu on 2024-07-25.
//

#ifndef INSPECTOR_H
#define INSPECTOR_H

#include <UiContainer.h>
#include <Component.h>
#include <Entity.h>

#include "ContextExplorer.h"

class ContextExplorer;

class EntityInspector : public UiContainer
{
public:
    void load_entity(Entity *entity);
    static EntityInspector *create(UiContainer *parent, ContextExplorer *explorer);

private:
    Entity *entity;
    ContextExplorer *explorer;
};

#endif //INSPECTOR_H
