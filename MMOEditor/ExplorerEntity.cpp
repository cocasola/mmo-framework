//
// Created by Keegan Beaulieu on 2024-07-27.
//

#include "ExplorerEntity.h"

#include <Context.h>

ExplorerEntity *ExplorerEntity::create(ContextExplorer *parent, Entity *entity)
{
    ExplorerEntity *ui_entity = alloc<ExplorerEntity>(parent);

    ui_entity->size = Vector2i(UI_SIZE_GROW, UI_SIZE_FIT);
    ui_entity->layout = UI_RIGHT;
    ui_entity->alignment = UI_CENTER_LEFT;
    ui_entity->set_padding(5);

    UiContainer *icon = UiContainer::create(ui_entity);
    icon->texture = Texture::create({ .path = "entity_icon.png" });
    icon->size = Vector2i(30, 30);
    icon->set_margin(5);

    ui_entity->name_element = UiText::create(ui_entity);
    ui_entity->name_element->size = Vector2i(UI_SIZE_FIT, UI_SIZE_FIT);
    ui_entity->name_element->text = entity->name;
    ui_entity->name_element->set_margin(5);

    ui_entity->entity = entity;

    ui_entity->on_click.subscribe([](MouseEvent event, void *data) -> void {
        ExplorerEntity *explorer_entity = (ExplorerEntity *)event.container;
        ContextExplorer *explorer = (ContextExplorer *)data;
        explorer->editor->select_entity(explorer_entity->entity);
    }, parent);

    parent->entity_explorer_map[entity] = ui_entity;

    return ui_entity;
}
