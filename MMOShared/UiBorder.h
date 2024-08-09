//
// Created by Keegan Beaulieu on 2024-07-30.
//

#ifndef UIBORDER_H
#define UIBORDER_H

#include "Colour.h"

struct UiBorder
{
    Colour colour;
    int size;

    UiBorder() : size(0) { }
    UiBorder(Colour colour, int size) : size(size), colour(colour) { }
};

#endif //UIBORDER_H
