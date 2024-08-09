//
// Created by Keegan Beaulieu on 2024-07-21.
//

#ifndef COLOUR_H
#define COLOUR_H

struct Colour
{
    float r;
    float g;
    float b;
    float a;

    Colour() : r(0), g(0), b(0), a(0) { }
    Colour(float r, float g, float b, float a) : r(r), b(b), g(g), a(a) { }
};

#endif //COLOUR_H
