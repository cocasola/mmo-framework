//
// Created by Keegan Beaulieu on 2024-08-08.
//

#include "UiPopup.h"

UiPopup *UiPopup::create(const Settings &settings)
{
    UiPopup *popup = alloc<UiPopup>(root_container);
    popup->positioning = UI_RELATIVE;
    popup->depth = DEPTH;
    popup->size = Vector2i(UI_SIZE_GROW, UI_SIZE_GROW);

    if (settings.close_off_focus) {
        UiContainer *close_clicker = UiContainer::create(popup);
        close_clicker->positioning = UI_RELATIVE;
        close_clicker->size = Vector2i(UI_SIZE_GROW, UI_SIZE_GROW);
        close_clicker->depth = -1;

        close_clicker->on_click.subscribe([](MouseEvent event, void *data) {
            UiPopup *popup = (UiPopup *)data;
            destroy(popup);
        }, popup);
    }

    return popup;
}
