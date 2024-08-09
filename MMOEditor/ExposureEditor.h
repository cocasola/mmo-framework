//
// Created by Keegan Beaulieu on 2024-08-09.
//

#ifndef EXPOSUREEDITOR_H
#define EXPOSUREEDITOR_H

#include <UiContainer.h>

class ExposureEditor
{
public:
    template <typename T>
    static void register_type(void(*renderer)(UiContainer *parent, T &data));

    static void init();

private:
    static std::unordered_map<>

    static void call_renderer(const std::type_info *type);
};

template<typename T>
void ExposureEditor::register_type(void(*renderer)(UiContainer *parent, T &data))
{

}

#endif //EXPOSUREEDITOR_H
