//
// Created by Keegan Beaulieu on 2024-08-01.
//

#include "UiButton.h"

UiButton *UiButton::create(UiContainer *parent)
{
    UiButton *button = alloc<UiButton>(parent);
    button->size.y = 50;
    button->left_padding = 20;
    button->right_padding = 20;
    button->alignment = UI_CENTER;
    button->set_style_tags({ "button" });

    button->text = UiText::create(button);
    button->text->set_style_tags({ "button-text" });

    return button;
}