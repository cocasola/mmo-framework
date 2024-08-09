//
// Created by Keegan Beaulieu on 2024-07-25.
//

#ifndef UICONTEXTMENU_H
#define UICONTEXTMENU_H

#include <vector>

#include "UiContainer.h"

class UiContextMenu
{
public:
    struct Option
    {
        std::string text;
        Texture *icon;
        void(*callback)(void *);
        void *data;
    };

    static const int DEPTH = 8000;

    static void show(const std::vector<std::vector<Option>> &options, UiContainer::MouseEvent event);

private:
    static UiContainer *context_menu;
    static bool hide_guard;
    static std::vector<std::vector<Option>> options;

    static void create_context_menu();
};

#endif //UICONTEXTMENU_H
