//
// Created by Keegan Beaulieu on 2024-08-09.
//

#include "ExposureEditor.h"

#include <UiText.h>

void ExposureEditor::init()
{
    register_type<Mesh *>([](UiContainer *parent, Mesh *&data) {
        UiText *test = UiText::create(parent);
        test->text = "Hello world123132";
    });
}
