//
// Created by Keegan Beaulieu on 2024-07-09.
//

#ifndef EVENT_H
#define EVENT_H

#include <List.h>

template <class T>
class Event
{
public:
    typedef void(*Handler)(T event_data, void *subscription_data);

    struct Subscription
    {
        void *data;
        Handler handler;
    };

    Subscription *subscribe(Handler handler, void *data);
    void unsubscribe(Subscription *subscription);
    void fire(T data);

    static void create(Event &event);
    static void destroy(Event &event);

    List<Subscription> subscriptions;
};

template <typename T>
Event<T>::Subscription *Event<T>::subscribe(Handler handler, void *data)
{
    Subscription *subscription = subscriptions.alloc({
        .data = data,
        .handler = (Handler)handler
    });

    return subscription;
}

template<typename T>
void Event<T>::unsubscribe(Subscription *subscription)
{
    subscriptions.destroy(subscription);
}

template<class T>
void Event<T>::fire(T data)
{
    for (auto i : subscriptions) {
        i.handler(data, i.data);
    }
}

template<class T>
void Event<T>::create(Event &event)
{

}

template<class T>
void Event<T>::destroy(Event &event)
{
    event.subscriptions.clear();
}

#endif //EVENT_H
