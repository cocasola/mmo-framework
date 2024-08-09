//
// Created by Keegan Beaulieu on 2024-07-19.
//

#ifndef COMPONENTMESSAGEHANDLER_H
#define COMPONENTMESSAGEHANDLER_H

#include <ComponentMessage.h>
#include <Session.h>

class ComponentMessageHandler
{
public:
    static void handler(ComponentMessage *message, Session *session);
};

#endif //COMPONENTMESSAGEHANDLER_H
