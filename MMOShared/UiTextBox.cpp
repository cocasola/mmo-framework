//
// Created by Keegan Beaulieu on 2024-07-30.
//

#include "UiTextBox.h"

#include <UiStyle.h>

UiTextBox *UiTextBox::create(UiContainer *parent)
{
    UiTextBox *box = alloc<UiTextBox>(parent);
    box->top_padding = 5;
    box->bottom_padding = 5;
    box->left_padding = 10;
    box->right_padding = 10;
    box->set_style_tags({ "text-box" });
    box->on_focus.subscribe(&handle_focus, nullptr);
    box->off_focus.subscribe(&handle_off_focus, nullptr);
    box->on_key_down.subscribe(&handle_key_press, nullptr);
    box->on_key_repeat.subscribe(&handle_key_press, nullptr);

    box->text = UiText::create(box);

    UiStyle text_style = UiStyle::get_style<UiText>({});
    box->caret = UiContainer::create(box);
    box->caret->size = Vector2i(2, 0);
    box->caret->positioning = UI_RELATIVE;
    box->caret->depth = 3;
    box->caret->hide = true;
    box->caret->set_style_tags({ "text-box-caret" });

    box->on_destroy.subscribe(&handle_destroy, nullptr);

    return box;
}

void UiTextBox::update_caret()
{
    int index = caret_position;
    bool last = false;

    if (index >= text->text.length()) {
        index = text->text.length() - 1;
        last = true;
    }

    caret->size.y = -text->ascent + text->descent;

    if (index == -1) {
        caret->position = text->calculated_position - calculated_position;
        caret->update();
        return;
    }

    UiText::CharacterDisplay c = text->text_display[index];
    caret->position.y = text->calculated_position.y - calculated_position.y;
    caret->position.x = c.position.x - c.bearing.x - calculated_position.x;

    if (last)
        caret->position.x += c.size.x;

    caret->update();
}

void UiTextBox::handle_focus(UiContainer *container, void *)
{
    UiTextBox *box = (UiTextBox *)container;
    box->caret_position = box->text->text.length();
    box->caret->hide = false;
    box->update_caret();
}

void UiTextBox::handle_off_focus(UiContainer *container, void *)
{
    UiTextBox *box = (UiTextBox *)container;
    box->caret->hide = true;
    box->on_edit.fire(box);
}

void UiTextBox::handle_key_press(KeyEvent event, void *)
{
    UiTextBox *box = (UiTextBox *)event.container;

    bool capitalize = false;

    if (Window::key_state[VK_CTRL])
        return;
    if (Window::key_state[VK_ALT])
        return;
    if (Window::key_state[VK_SUPER])
        return;
    if (Window::key_state[VK_SHIFT])
        capitalize = true;

    switch (event.key) {
        case VK_1:
            box->text->text.insert(box->caret_position, 1, '1');
            ++box->caret_position;
        break;
        case VK_2:
            box->text->text.insert(box->caret_position, 1, '2');
            ++box->caret_position;
        break;
        case VK_3:
            box->text->text.insert(box->caret_position, 1, '3');
            ++box->caret_position;
        break;
        case VK_4:
            box->text->text.insert(box->caret_position, 1, '4');
            ++box->caret_position;
        break;
        case VK_5:
            box->text->text.insert(box->caret_position, 1, '5');
            ++box->caret_position;
        break;
        case VK_6:
            box->text->text.insert(box->caret_position, 1, '6');
            ++box->caret_position;
        break;
        case VK_7:
            box->text->text.insert(box->caret_position, 1, '7');
            ++box->caret_position;
        break;
        case VK_8:
            box->text->text.insert(box->caret_position, 1, '8');
            ++box->caret_position;
        break;
        case VK_9:
            box->text->text.insert(box->caret_position, 1, '9');
            ++box->caret_position;
        break;
        case VK_0:
            box->text->text.insert(box->caret_position, 1, '0');
            ++box->caret_position;
        break;
        case VK_Q:
            if (capitalize)
                box->text->text.insert(box->caret_position, 1, 'Q');
            else
                box->text->text.insert(box->caret_position, 1, 'q');
            ++box->caret_position;
        break;
        case VK_W:
            if (capitalize)
                box->text->text.insert(box->caret_position, 1, 'W');
            else
                box->text->text.insert(box->caret_position, 1, 'w');
            ++box->caret_position;
        break;
        case VK_E:
            if (capitalize)
                box->text->text.insert(box->caret_position, 1, 'E');
            else
                box->text->text.insert(box->caret_position, 1, 'e');
            ++box->caret_position;
        break;
        case VK_R:
            if (capitalize)
                box->text->text.insert(box->caret_position, 1, 'R');
            else
                box->text->text.insert(box->caret_position, 1, 'r');
            ++box->caret_position;
        break;
        case VK_T:
            if (capitalize)
                box->text->text.insert(box->caret_position, 1, 'T');
            else
                box->text->text.insert(box->caret_position, 1, 't');
            ++box->caret_position;
        break;
        case VK_Y:
            if (capitalize)
                box->text->text.insert(box->caret_position, 1, 'Y');
            else
                box->text->text.insert(box->caret_position, 1, 'y');
            ++box->caret_position;
        break;
        case VK_U:
            if (capitalize)
                box->text->text.insert(box->caret_position, 1, 'U');
            else
                box->text->text.insert(box->caret_position, 1, 'u');
            ++box->caret_position;
        break;
        case VK_I:
            if (capitalize)
                box->text->text.insert(box->caret_position, 1, 'I');
            else
                box->text->text.insert(box->caret_position, 1, 'i');
            ++box->caret_position;
        break;
        case VK_O:
            if (capitalize)
                box->text->text.insert(box->caret_position, 1, 'O');
            else
                box->text->text.insert(box->caret_position, 1, 'o');
            ++box->caret_position;
        break;
        case VK_P:
            if (capitalize)
                box->text->text.insert(box->caret_position, 1, 'P');
            else
                box->text->text.insert(box->caret_position, 1, 'p');
            ++box->caret_position;
        break;
        case VK_A:
            if (capitalize)
                box->text->text.insert(box->caret_position, 1, 'A');
            else
                box->text->text.insert(box->caret_position, 1, 'a');
            ++box->caret_position;
        break;
        case VK_S:
            if (capitalize)
                box->text->text.insert(box->caret_position, 1, 'S');
            else
                box->text->text.insert(box->caret_position, 1, 's');
            ++box->caret_position;
        break;
        case VK_D:
            if (capitalize)
                box->text->text.insert(box->caret_position, 1, 'D');
            else
                box->text->text.insert(box->caret_position, 1, 'd');
            ++box->caret_position;
        break;
        case VK_F:
            if (capitalize)
                box->text->text.insert(box->caret_position, 1, 'F');
            else
                box->text->text.insert(box->caret_position, 1, 'f');
            ++box->caret_position;
        break;
        case VK_G:
            if (capitalize)
                box->text->text.insert(box->caret_position, 1, 'G');
            else
                box->text->text.insert(box->caret_position, 1, 'g');
            ++box->caret_position;
        break;
        case VK_H:
            if (capitalize)
                box->text->text.insert(box->caret_position, 1, 'H');
            else
                box->text->text.insert(box->caret_position, 1, 'h');
            ++box->caret_position;
        break;
        case VK_J:
            if (capitalize)
                box->text->text.insert(box->caret_position, 1, 'J');
            else
                box->text->text.insert(box->caret_position, 1, 'j');
            ++box->caret_position;
        break;
        case VK_K:
            if (capitalize)
                box->text->text.insert(box->caret_position, 1, 'K');
            else
                box->text->text.insert(box->caret_position, 1, 'k');
            ++box->caret_position;
        break;
        case VK_L:
            if (capitalize)
                box->text->text.insert(box->caret_position, 1, 'L');
            else
                box->text->text.insert(box->caret_position, 1, 'l');
            ++box->caret_position;
        break;
        case VK_Z:
            if (capitalize)
                box->text->text.insert(box->caret_position, 1, 'Z');
            else
                box->text->text.insert(box->caret_position, 1, 'z');
            ++box->caret_position;
        break;
        case VK_X:
            if (capitalize)
                box->text->text.insert(box->caret_position, 1, 'X');
            else
                box->text->text.insert(box->caret_position, 1, 'x');
            ++box->caret_position;
        break;
        case VK_C:
            if (capitalize)
                box->text->text.insert(box->caret_position, 1, 'C');
            else
                box->text->text.insert(box->caret_position, 1, 'c');
            ++box->caret_position;
        break;
        case VK_V:
            if (capitalize)
                box->text->text.insert(box->caret_position, 1, 'V');
            else
                box->text->text.insert(box->caret_position, 1, 'v');
            ++box->caret_position;
        break;
        case VK_B:
            if (capitalize)
                box->text->text.insert(box->caret_position, 1, 'B');
            else
                box->text->text.insert(box->caret_position, 1, 'b');
            ++box->caret_position;
        break;
        case VK_N:
            if (capitalize)
                box->text->text.insert(box->caret_position, 1, 'N');
            else
                box->text->text.insert(box->caret_position, 1, 'n');
            ++box->caret_position;
        break;
        case VK_M:
            if (capitalize)
                box->text->text.insert(box->caret_position, 1, 'M');
            else
                box->text->text.insert(box->caret_position, 1, 'm');
            ++box->caret_position;
        break;
        case VK_SPACE:
            box->text->text.insert(box->caret_position, 1, ' ');
            ++box->caret_position;
        break;
        case VK_BACKSPACE:
            if (box->caret_position == 0)
                break;

            box->text->text.erase(box->caret_position - 1, 1);
            --box->caret_position;
        break;
        case VK_RIGHT:
            ++box->caret_position;
            if (box->caret_position > box->text->text.length())
                --box->caret_position;
        break;
        case VK_LEFT:
            --box->caret_position;
            if (box->caret_position < 0)
            ++box->caret_position;
        break;
        default:
        break;
    }

    box->text->update();
    box->update_caret();
}

void UiTextBox::handle_destroy(UiContainer *container, void *)
{
    UiTextBox *box = (UiTextBox *)container;
    Event<UiTextBox *>::destroy(box->on_edit);
}
