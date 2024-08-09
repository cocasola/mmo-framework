//
// Created by Keegan Beaulieu on 2024-08-08.
//

#ifndef UISEARCHBOX_H
#define UISEARCHBOX_H

#include "UiContainer.h"
#include "UiText.h"

template <typename T>
class UiSearchBox : public UiContainer
{
public:
    struct Item
    {
        typedef void(*Callback)(T &item);

        Texture *icon;
        std::string name;
        Callback on_click;
        T data;
    };

    void set_items(std::vector<Item> items);

    static UiSearchBox *create(UiContainer *parent);

private:
    List<Item> items;

    UiContainer *results;
};

template<typename T>
void UiSearchBox<T>::set_items(std::vector<Item> items)
{
    this->items.clear();

    for (auto &item : items) {
        this->items.alloc(item);
    }

    results->delete_children();

    for (auto &item : this->items) {
        UiContainer *item_element = UiContainer::create(results);
        item_element->size = Vector2i(UI_SIZE_GROW, UI_SIZE_FIT);
        item_element->layout = UI_RIGHT;
        item_element->alignment = UI_CENTER_LEFT;
        item_element->set_padding(5);

        UiContainer *icon_element = UiContainer::create(item_element);
        icon_element->set_margin(5);
        icon_element->size = Vector2i(30, 30);
        icon_element->texture = item.icon;

        UiText *text = UiText::create(item_element);
        text->set_margin(5);
        text->text = item.name;

        if (item.on_click) {
            item_element->set_style_tags({ "clickable-search-box-item" });

            item_element->on_click.subscribe([](MouseEvent event, void *data) {
                Item *item = (Item *)data;
                item->on_click(item->data);
            }, &item);
        }
    }
}

template <typename T>
UiSearchBox<T> *UiSearchBox<T>::create(UiContainer *parent)
{
    UiSearchBox *search_box = alloc<UiSearchBox>(parent);

    search_box->results = UiContainer::create(search_box);
    search_box->results->size = Vector2i(UI_SIZE_GROW, UI_SIZE_GROW);

    return search_box;
}

#endif //UISEARCHBOX_H
