//
// Created by Keegan Beaulieu on 2024-08-01.
//

#include "UiHLine.h"

UiHLine *UiHLine::create(UiContainer *parent)
{
    UiHLine *line = alloc<UiHLine>(parent);
    line->size = Vector2i(UI_SIZE_GROW, 2);
    line->set_style_tags({ "h-line" });

    return line;
}
