//
// Created by Keegan Beaulieu on 2024-07-14.
//

#ifndef CLIENTCONNECTHANDLER_H
#define CLIENTCONNECTHANDLER_H

#include <NetworkMessage.h>

#include <ClientSession.h>

class ClientConnectHandler
{
public:
    static void handler(ClientConnect *, Session *session);
};

#endif //CLIENTCONNECTHANDLER_H
