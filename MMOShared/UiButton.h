//
// Created by Keegan Beaulieu on 2024-08-01.
//

#ifndef UIBUTTON_H
#define UIBUTTON_H

#include "UiContainer.h"
#include "UiText.h"

class UiButton : public UiContainer
{
public:
    UiText *text;

    static UiButton *create(UiContainer *parent);
};

#endif //UIBUTTON_H
