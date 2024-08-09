//
// Created by Keegan Beaulieu on 2024-07-25.
//

#include "EntityInspector.h"

#include <UiButton.h>
#include <UiHLine.h>
#include <UiPopup.h>
#include <UiSearchBox.h>
#include <UiText.h>
#include <UiTextBox.h>

#include "Icon.h"

struct ComponentOption
{
    Component::Descriptor<Component> *descriptor;
    UiSearchBox<ComponentOption> *search_box;
    EntityInspector *inspector;
};

void EntityInspector::load_entity(Entity *entity)
{
    delete_children();

    this->entity = entity;

    UiContainer *entity_name = UiContainer::create(this);
    entity_name->set_padding(10);
    entity_name->layout = UI_RIGHT;
    entity_name->alignment = UI_CENTER_LEFT;
    entity_name->size.x = UI_SIZE_GROW;

    UiContainer *icon = UiContainer::create(entity_name);
    icon->size = Vector2i(35, 35);
    icon->texture = Icon::entity;
    icon->set_margin(10);

    UiTextBox *text_box = UiTextBox::create(entity_name);
    text_box->set_margin(10);
    text_box->text->text = entity->name;
    text_box->size.x = UI_SIZE_GROW;

    text_box->on_edit.subscribe([](UiTextBox *text_box, void *data) -> void {
        EntityInspector *inspector = (EntityInspector *)data;
        inspector->entity->name = text_box->text->text;

        UiText *explorer_name_element = inspector->explorer->entity_explorer_map[inspector->entity]->name_element;
        explorer_name_element->text = text_box->text->text;
        explorer_name_element->update();
    }, this);

    UiContainer *transform = UiContainer::create(this);
    transform->set_padding(10);
    transform->layout = UI_RIGHT;
    transform->size.x = UI_SIZE_GROW;

    UiContainer *transform_members_names = UiContainer::create(transform);

    UiContainer *grid_dummy = UiContainer::create(transform_members_names);
    grid_dummy->size.y = 40;

    UiContainer *position = UiContainer::create(transform_members_names);
    position->set_margin(10);
    UiText *position_text = UiText::create(position);
    position_text->text = "Position";

    UiContainer *rotation = UiContainer::create(transform_members_names);
    rotation->set_margin(10);
    UiText *rotation_text = UiText::create(rotation);
    rotation_text->text = "Rotation";

    UiContainer *scale = UiContainer::create(transform_members_names);
    scale->set_margin(10);
    UiText *scale_text = UiText::create(scale);
    scale_text->text = "Scale";

    UiContainer *transform_values = UiContainer::create(transform);
    transform_values->size.x = UI_SIZE_GROW;

    UiContainer *vector_member_names = UiContainer::create(transform_values);
    vector_member_names->layout = UI_RIGHT;
    vector_member_names->size.x = UI_SIZE_GROW;
    vector_member_names->size.y = 40;

    UiText *x = UiText::create(vector_member_names);
    x->size = Vector2i(UI_SIZE_GROW, UI_SIZE_GROW);
    x->text = "x";
    x->alignment = UI_CENTER;

    UiText *y = UiText::create(vector_member_names);
    y->size = Vector2i(UI_SIZE_GROW, UI_SIZE_GROW);
    y->text = "y";
    y->alignment = UI_CENTER;

    UiText *z = UiText::create(vector_member_names);
    z->size = Vector2i(UI_SIZE_GROW, UI_SIZE_GROW);
    z->text = "z";
    z->alignment = UI_CENTER;

    UiContainer *position_values = UiContainer::create(transform_values);
    position_values->layout = UI_RIGHT;
    position_values->size.x = UI_SIZE_GROW;

    UiTextBox *position_x = UiTextBox::create(position_values);
    position_x->set_margin(5);
    position_x->size.x = UI_SIZE_GROW;
    position_x->text->text = std::to_string(entity->transform->position.x);
    UiTextBox *position_y = UiTextBox::create(position_values);
    position_y->set_margin(5);
    position_y->size.x = UI_SIZE_GROW;
    position_y->text->text = std::to_string(entity->transform->position.y);
    UiTextBox *position_z = UiTextBox::create(position_values);
    position_z->set_margin(5);
    position_z->size.x = UI_SIZE_GROW;
    position_z->text->text = std::to_string(entity->transform->position.z);

    UiContainer *rotation_values = UiContainer::create(transform_values);
    rotation_values->layout = UI_RIGHT;
    rotation_values->size.x = UI_SIZE_GROW;

    UiTextBox *rotation_x = UiTextBox::create(rotation_values);
    rotation_x->set_margin(5);
    rotation_x->size.x = UI_SIZE_GROW;
    UiTextBox *rotation_y = UiTextBox::create(rotation_values);
    rotation_y->set_margin(5);
    rotation_y->size.x = UI_SIZE_GROW;
    UiTextBox *rotation_z = UiTextBox::create(rotation_values);
    rotation_z->set_margin(5);
    rotation_z->size.x = UI_SIZE_GROW;

    UiContainer *scale_values = UiContainer::create(transform_values);
    scale_values->layout = UI_RIGHT;
    scale_values->size.x = UI_SIZE_GROW;

    UiTextBox *scale_x = UiTextBox::create(scale_values);
    scale_x->set_margin(5);
    scale_x->size.x = UI_SIZE_GROW;
    UiTextBox *scale_y = UiTextBox::create(scale_values);
    scale_y->set_margin(5);
    scale_y->size.x = UI_SIZE_GROW;
    UiTextBox *scale_z = UiTextBox::create(scale_values);
    scale_z->set_margin(5);
    scale_z->size.x = UI_SIZE_GROW;

    UiHLine *line = UiHLine::create(this);
    line->top_margin = 10;
    line->bottom_margin = 10;

    for (auto &component_ref : entity->components) {
        UiContainer *component = UiContainer::create(this);
        component->set_padding(10);

        UiContainer *title = UiContainer::create(component);
        title->layout = UI_RIGHT;
        title->size.x = UI_SIZE_GROW;
        title->alignment = UI_CENTER_LEFT;

        UiContainer *icon = UiContainer::create(title);
        icon->set_margin(10);
        icon->size = Vector2i(25, 25);
        icon->texture = Icon::entity;

        UiText *name = UiText::create(title);
        name->text = component_ref.descriptor->name;
        name->set_margin(10);

        UiContainer *exposures = UiContainer::create(component);
        exposures->set_padding(10);

        for (auto &exposure : component_ref.descriptor->expose) {

        }
    }

    UiContainer *add_component_wrapper = UiContainer::create(this);
    add_component_wrapper->set_padding(10);
    add_component_wrapper->alignment = UI_CENTER;
    add_component_wrapper->size.x = UI_SIZE_GROW;

    UiButton *add_component = UiButton::create(add_component_wrapper);
    add_component->text->text = "Add Component";
    // ReSharper disable once CppDFAUnusedValue
    add_component->on_click.subscribe([](MouseEvent event, void *data) {
        UiButton *add_component = (UiButton *)event.container;

        UiSearchBox<ComponentOption> *search_box = UiSearchBox<ComponentOption>::create(add_component);
        search_box->size = Vector2i(400, 600);
        search_box->positioning = UI_RELATIVE;
        search_box->depth = 10;
        search_box->set_style_tags({ "popup-box" });

        search_box->on_click.subscribe([](MouseEvent, void *) { }, nullptr);

        auto update_subscription = add_component->on_update.subscribe([](UiContainer *add_component, void *data) {
            auto search_box = (UiSearchBox<ComponentOption> *)data;
            search_box->position.y = add_component->calculated_size.y;
            search_box->position.x -= (search_box->size.x - add_component->calculated_size.x)/2;
        }, search_box);

        search_box->on_destroy.subscribe([](UiContainer *search_box, void *data) {
            auto subscription = (Event<UiContainer *>::Subscription *)data;
            search_box->parent->on_update.unsubscribe(subscription);
        }, update_subscription);

        auto static_on_click_subscription = static_on_click.subscribe([](MouseEvent event, void *data) {
            auto search_box = (UiSearchBox<ComponentOption> *)data;

            if (event.container && (event.container == search_box || event.container->is_child_of(search_box)))
                return;

            defer([](void *data) {
                auto search_box = (UiSearchBox<ComponentOption> *)data;
                destroy(search_box);
            }, search_box);
        }, search_box);

        search_box->on_destroy.subscribe([](UiContainer *search_box, void *data) {
            auto subscription = (Event<MouseEvent>::Subscription *)data;
            static_on_click.unsubscribe(subscription);
        }, static_on_click_subscription);

        std::vector<UiSearchBox<ComponentOption>::Item> components;

        for (auto &descriptor : Component::descriptors) {
            components.push_back(
                {
                    .name = descriptor.name,
                    .on_click = [](ComponentOption &option) {
                        Component::Descriptor<Component> *descriptor = option.descriptor;
                        Entity *entity = option.inspector->entity;

                        Component *instance = Component::instance(descriptor, entity->context);
                        entity->components.push_back({
                            .descriptor = descriptor,
                            .instance = instance
                        });

                        defer([](void *data) {
                            auto inspector = (EntityInspector *)data;
                            inspector->load_entity(inspector->entity);
                        }, option.inspector);
                    },
                    .data = {
                        &descriptor,
                        search_box,
                        (EntityInspector *)data
                    },
                    .icon = Icon::entity
                }
            );
        }

        search_box->set_items(components);

        add_component->update();
    }, this);

    update();
}

EntityInspector *EntityInspector::create(UiContainer *parent, ContextExplorer *explorer)
{
    EntityInspector *inspector = alloc<EntityInspector>(parent);

    inspector->size.x = 600;
    inspector->size.y = UI_SIZE_GROW;
    inspector->style_tags = { "bg-light" };
    inspector->explorer = explorer;

    return inspector;
}
