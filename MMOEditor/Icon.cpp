//
// Created by Keegan Beaulieu on 2024-07-30.
//

#include "Icon.h"

Texture *Icon::entity = nullptr;
Texture *Icon::plus = nullptr;

void Icon::init()
{
    entity = Texture::create({
        .path = "entity_icon.png"
    });

    plus = Texture::create({
        .path = "plus_icon.png"
    });
}
