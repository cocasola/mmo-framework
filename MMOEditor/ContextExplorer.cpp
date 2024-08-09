//
// Created by Keegan Beaulieu on 2024-07-25.
//

#include "ContextExplorer.h"

#include <Context.h>
#include <UiContextMenu.h>
#include <UiText.h>

#include "ExplorerEntity.h"
#include "Icon.h"

ContextExplorer *ContextExplorer::create(ContextEditor *editor)
{
    ContextExplorer *explorer = alloc<ContextExplorer>(editor);

    explorer->entity_explorer_map = std::unordered_map<Entity *, ExplorerEntity *>();
    explorer->editor = editor;

    explorer->size.x = 600;
    explorer->size.y = UI_SIZE_GROW;
    explorer->style_tags = { "bg-light" };

    explorer->on_click.subscribe([](MouseEvent event, void *explorer) -> void {
        if (event.mouse_button == MOUSE_RIGHT) {
            UiContextMenu::show({{
                {
                    .text = "Create Entity",
                    .callback = &create_entity,
                    .data = explorer,
                    .icon = Icon::plus
                }
            }}, event);
        }
    }, explorer);

    return explorer;
}

void ContextExplorer::create_entity(void *data)
{
    ContextExplorer *explorer = (ContextExplorer *)data;
    Entity *entity = explorer->editor->create_entity();
    explorer->editor->select_entity(entity);
}

void ContextExplorer::handle_destroy(UiContainer *container, void *)
{
    ContextExplorer *explorer = (ContextExplorer *)container;
    explorer->entity_explorer_map.~unordered_map();
}
