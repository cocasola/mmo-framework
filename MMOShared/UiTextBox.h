//
// Created by Keegan Beaulieu on 2024-07-30.
//

#ifndef UITEXTBOX_H
#define UITEXTBOX_H

#include "UiText.h"

class UiTextBox : public UiContainer
{
public:
    UiText *text;
    UiContainer *caret;
    int caret_position;
    Event<UiTextBox *> on_edit;

    static UiTextBox *create(UiContainer *parent);

private:
    void update_caret();
    static void handle_focus(UiContainer *container, void *);
    static void handle_off_focus(UiContainer *container, void *);
    static void handle_key_press(KeyEvent event, void *);
    static void handle_destroy(UiContainer *container, void *);
};

#endif //UITEXTBOX_H
