//
// Created by Keegan Beaulieu on 2024-07-24.
//

#include "Style.h"

#include <UiContainer.h>
#include <UiStyle.h>
#include <UiText.h>

void Style::register_styles()
{
    UiStyle bg_light;
    bg_light.add("background-colour", Colour(0.9, 0.9, 0.9, 1.0));
    UiStyle::register_tag("bg-light", bg_light);

    UiStyle text;
    text.add("text-colour", Colour(0.1, 0.1, 0.15, 1.0));
    text.add("font", std::string("SFNSRounded.ttf"));
    text.add("font-size", 30);
    UiStyle::register_class<UiText>(text);

    UiStyle popup_box;
    popup_box.add("background-colour", Colour(0.95f, 0.95f, 0.95f, 1.0f));
    popup_box.add("shadow", UiShadow(Colour(0.0f, 0.0f, 0.0f, 0.20f), 5, 32));
    popup_box.add("border", UiBorder(Colour(0.7f, 0.7f, 0.7f, 1.0f), 1));
    UiStyle::register_tag("popup-box", popup_box);
}
