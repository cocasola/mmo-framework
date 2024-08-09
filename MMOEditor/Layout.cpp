//
// Created by Keegan Beaulieu on 2024-07-21.
//

#include "Layout.h"

#include <UiStyle.h>

#include "ContextEditor.h"

void Layout::init()
{
    ContextEditor *editor = ContextEditor::create(UiContainer::root_container);
    ContextEditor::current = editor;

    UiStyle debug;
    debug.add("background-colour", Colour(1.0f, 0.0f, 0.0f, 1.0f));
    UiStyle::register_tag("debug", debug);

    editor->update();
}

void Layout::cleanup()
{

}
