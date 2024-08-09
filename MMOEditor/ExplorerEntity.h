//
// Created by Keegan Beaulieu on 2024-07-27.
//

#ifndef EXPLORERENTITY_H
#define EXPLORERENTITY_H

#include <UiContainer.h>
#include <Component.h>
#include <Entity.h>
#include <UiText.h>

#include "ContextExplorer.h"

class ContextExplorer;

class ExplorerEntity : public UiContainer
{
public:
    Entity *entity;
    UiText *name_element;

    static ExplorerEntity *create(ContextExplorer *parent, Entity *entity);
};

#endif //EXPLORERENTITY_H
