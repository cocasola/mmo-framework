//
// Created by Keegan Beaulieu on 2024-07-25.
//

#include "UiContextMenu.h"

#include "UiStyle.h"
#include "UiContainer.h"
#include "UiText.h"
#include "UiShadow.h"

UiContainer *UiContextMenu::context_menu = nullptr;
bool UiContextMenu::hide_guard = false;
std::vector<std::vector<UiContextMenu::Option>> UiContextMenu::options;

void UiContextMenu::show(const std::vector<std::vector<Option>> &_options, UiContainer::MouseEvent event)
{
    if (!context_menu)
        create_context_menu();
    else
        context_menu->delete_children();

    options = _options;

    for (auto &option_group : options) {
        for (auto &option : option_group) {
            UiContainer *child = UiContainer::create(context_menu);
            child->size.x = 500;
            child->size.y = UI_SIZE_FIT;
            child->layout = UI_RIGHT;
            child->alignment = UI_CENTER_LEFT;
            child->set_padding(8);

            UiContainer *icon = UiContainer::create(child);
            icon->size = Vector2i(20, 20);
            icon->texture = option.icon;
            icon->set_margin(8);

            UiText *text = UiText::create(child);
            text->text = option.text;
            text->alignment = UI_TOP_LEFT;
            text->set_margin(8);

            child->on_click.subscribe([](UiContainer::MouseEvent event, void *data) -> void {
                if (event.mouse_button != MOUSE_LEFT)
                    return;

                Option *option_ptr = (Option *)data;
                option_ptr->callback(option_ptr->data);
            }, &option);
        }
    }

    context_menu->position = event.position;
    context_menu->update();

    hide_guard = true;
    context_menu->hide = false;
}

void UiContextMenu::create_context_menu()
{
    UiContainer *wrapper = UiContainer::create(UiContainer::root_container);

    context_menu = UiContainer::create(wrapper);
    context_menu->positioning = UI_ABSOLUTE;
    context_menu->depth = DEPTH;
    context_menu->style_tags = { "context-menu" };

    UiStyle style;
    style.add("background-colour", Colour(0.95f, 0.95f, 0.95f, 1.0f));
    style.add("shadow", UiShadow(Colour(0.0f, 0.0f, 0.0f, 0.25f), 10, 32));
    style.add("corner-radius", 10.0f);
    style.add("border", UiBorder(Colour(0.7f, 0.7f, 0.7f, 1.0f), 1));
    UiStyle::register_tag("context-menu", style);

    Window::on_click.subscribe([](Window::MouseEvent event, void *) -> void {
        if (hide_guard) {
            hide_guard = false;
            return;
        }

        context_menu->hide = true;
    }, nullptr);
}
