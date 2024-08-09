//
// Created by Keegan Beaulieu on 2024-07-24.
//

#ifndef UISTYLE_H
#define UISTYLE_H

#include <string>
#include <format>
#include <typeindex>
#include <UiBorder.h>
#include <unordered_map>
#include <Window.h>

#include "Colour.h"
#include "UiShadow.h"

enum UiStyleState
{
    UI_HOVERED,
    UI_PRESSED
};

class UiStyle
{
public:
    struct Value
    {
        enum Type
        {
            COLOUR,
            STRING,
            INT,
            UI_SHADOW,
            FLOAT,
            UI_BORDER,
            UI_CURSOR
        };

        void *data;
        Type type;
    };

    typedef std::unordered_map<std::string, Value> Values;

    std::unordered_map<std::string, Value> values;
    std::unordered_map<UiStyleState, Values> state_values;

    template <typename T>
    void add(const std::string &key, const T &value);

    template <typename T>
    void add_state(UiStyleState state, const std::string &key, const T &value);

    template <typename T>
    T &get(const std::string &key);

    template <typename T>
    T &get_state(UiStyleState state, const std::string &key);

    template <typename T>
    static void register_class(const UiStyle &style);

    template <typename T>
    static UiStyle get_style(const std::vector<std::string> &tags);

    static void register_tag(const std::string &tag, const UiStyle &style);
    static void init();
    static void cleanup();

private:
    template <typename T>
    void add_to(Values &values, const std::string &key, const T &value);

    template <typename T>
    T &get_from(Values &values, const std::string &key) const;

    static std::unordered_map<std::type_index, UiStyle> class_styles;
    static std::unordered_map<std::string, UiStyle> tag_styles;
};

const UiStyle EMPTY_STYLE{};

template<typename T>
void UiStyle::add(const std::string &key, const T &value)
{
    add_to(values, key, value);
}

template<typename T>
void UiStyle::add_state(UiStyleState state, const std::string &key, const T &value)
{
    add_to(state_values[state], key, value);
}

template<typename T>
T &UiStyle::get(const std::string &key)
{
    return get_from<T>(values, key);
}

template<typename T>
T &UiStyle::get_state(UiStyleState state, const std::string &key)
{
    return get_from<T>(state_values[state], key);
}

template <typename T>
void UiStyle::register_class(const UiStyle &style)
{
    std::type_index type_index = std::type_index(typeid(T));
    class_styles[type_index] = style;
}

template <typename T>
UiStyle UiStyle::get_style(const std::vector<std::string> &tags)
{
    UiStyle style = EMPTY_STYLE;

    try {
        std::type_index type_index = std::type_index(typeid(T));
        style = class_styles.at(type_index);
    } catch (...) { }

    for (int i = 0; i < tags.size(); ++i) {
        try {
            UiStyle tag_style = tag_styles.at(tags[i]);

            for (auto &value_pair : tag_style.values) {
                style.values[value_pair.first] = value_pair.second;
            }

            for (auto &state_pair : tag_style.state_values) {
                for (auto &value_pair : state_pair.second) {
                    style.state_values[state_pair.first][value_pair.first] = value_pair.second;
                }
            }
        } catch (...) { }
    }

    return style;
}

template<typename T>
void UiStyle::add_to(Values &values, const std::string &key, const T &value)
{
    Value::Type type;

    if (std::is_same<T, std::string>::value)
        type = Value::STRING;
    else if (std::is_same<T, Colour>::value)
        type = Value::COLOUR;
    else if (std::is_same<T, int>::value)
        type = Value::INT;
    else if (std::is_same<T, UiShadow>::value)
        type = Value::UI_SHADOW;
    else if (std::is_same<T, float>::value)
        type = Value::FLOAT;
    else if (std::is_same<T, UiBorder>::value)
        type = Value::UI_BORDER;
    else if (std::is_same<T, Window::Cursor *>::value)
        type = Value::UI_CURSOR;
    else
        throw std::runtime_error(std::format("Invalid style type: {}.", typeid(T).name()));

    void *data = calloc(1, sizeof(T));
    *(T *)data = value;

    values[key] = {
        .data = data,
        .type = type
    };
}

template<typename T>
T &UiStyle::get_from(Values &values, const std::string &key) const
{
    const Value &value = values.at(key);

    if (std::is_same<T, std::string>::value) {
        if (value.type != Value::STRING)
            throw std::runtime_error("Value is not a string.");
    } else if (std::is_same<T, Colour>::value) {
        if (value.type != Value::COLOUR)
            throw std::runtime_error("Value is not a colour.");
    } else if (std::is_same<T, int>::value) {
        if (value.type != Value::INT)
            throw std::runtime_error("Value is not an int.");
    } else if (std::is_same<T, UiShadow>::value) {
        if (value.type != Value::UI_SHADOW)
            throw std::runtime_error("Value is not a shadow.");
    } else if (std::is_same<T, float>::value) {
        if (value.type != Value::FLOAT)
            throw std::runtime_error("Value is not a float.");
    } else if (std::is_same<T, UiBorder>::value) {
        if (value.type != Value::UI_BORDER)
            throw std::runtime_error("Value is not a border.");
    } else if (std::is_same<T, Window::Cursor *>::value) {
        if (value.type != Value::UI_CURSOR)
            throw std::runtime_error("Value is not a cursor.");
    } else {
        throw std::runtime_error(std::format("Invalid style type: {}.", typeid(T).name()));
    }

    return *(T *)value.data;
}

#endif //UISTYLE_H
