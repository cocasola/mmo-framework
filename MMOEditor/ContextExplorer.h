//
// Created by Keegan Beaulieu on 2024-07-25.
//

#ifndef CONTEXTEXPLORER_H
#define CONTEXTEXPLORER_H

#include <UiContainer.h>
#include <Component.h>
#include <Entity.h>

#include "ContextEditor.h"
#include "ExplorerEntity.h"

class ContextEditor;
class ExplorerEntity;

class ContextExplorer : public UiContainer
{
public:
    std::unordered_map<Entity *, ExplorerEntity *> entity_explorer_map;
    ContextEditor *editor;

    static ContextExplorer *create(ContextEditor *editor);

private:
    static void create_entity(void *);
    static void handle_destroy(UiContainer *container, void *);
};

#endif //CONTEXTEXPLORER_H
