//
// Created by Keegan Beaulieu on 2024-07-26.
//

#ifndef UISHADOW_H
#define UISHADOW_H

#include "Colour.h"

struct UiShadow
{
    Colour colour;
    int size;
    int blur;

    UiShadow() : size(0) { }
    UiShadow(Colour colour, int size, int blur) : colour(colour), size(size), blur(blur) { }
};

#endif //UISHADOW_H
