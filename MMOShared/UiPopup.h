//
// Created by Keegan Beaulieu on 2024-08-08.
//

#ifndef UIPOPUP_H
#define UIPOPUP_H

#include "UiContainer.h"

class UiPopup : public UiContainer
{
public:
    struct Settings
    {
        bool close_off_focus = true;
    };

    static const int DEPTH = 10000;

    static UiPopup *create(const Settings &settings);

private:
    UiContainer *close_clicker;
};

#endif //UIPOPUP_H
