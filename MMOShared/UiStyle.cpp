//
// Created by Keegan Beaulieu on 2024-07-24.
//

#include "UiStyle.h"

#include <UiBorder.h>
#include <UiTextBox.h>

std::unordered_map<std::type_index, UiStyle> UiStyle::class_styles;
std::unordered_map<std::string, UiStyle> UiStyle::tag_styles;

void UiStyle::register_tag(const std::string &tag, const UiStyle &style)
{
    tag_styles[tag] = style;
}

void UiStyle::init()
{
    UiStyle text_box;
    text_box.add("background-colour", Colour(0.95f, 0.95f, 0.95f, 1.0f));
    text_box.add("corner-radius", 5.0f);
    text_box.add("border", UiBorder(Colour(0.7f, 0.7f, 0.7f, 1.0f), 1));
    text_box.add_state(UI_HOVERED, "cursor", Window::cursor_defaults.edit);
    register_tag("text-box", text_box);

    UiStyle text_box_caret;
    text_box_caret.add("background-colour", Colour(0.0f, 0.0f, 0.0f, 1.0f));
    register_tag("text-box-caret", text_box_caret);

    UiStyle hline_style;
    hline_style.add("background-colour", Colour(0.7f, 0.7f, 0.7f, 1.0f));
    register_tag("h-line", hline_style);

    UiStyle button;
    button.add("background-colour", Colour(0.95f, 0.95f, 0.95f, 1.0f));
    button.add("corner-radius", 25.0f);
    button.add("border", UiBorder(Colour(0.7f, 0.7f, 0.7f, 1.0f), 1));
    button.add_state(UI_HOVERED, "cursor", Window::cursor_defaults.point);
    register_tag("button", button);

    UiStyle button_text;
    button_text.add("font-size", 25);
    register_tag("button-text", button_text);

    UiStyle clickable_search_box_item;
    clickable_search_box_item.add_state(UI_HOVERED, "cursor", Window::cursor_defaults.point);
    register_tag("clickable-search-box-item", clickable_search_box_item);
}

void UiStyle::cleanup()
{
    for (auto style_pair : class_styles) {
        for (auto value_pair : style_pair.second.values) {
            free (value_pair.second.data);
        }
    }

    for (auto style_pair : tag_styles) {
        for (auto value_pair : style_pair.second.values) {
            free (value_pair.second.data);
        }
    }
}
