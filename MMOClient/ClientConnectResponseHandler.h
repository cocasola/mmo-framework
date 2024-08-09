//
// Created by Keegan Beaulieu on 2024-07-15.
//

#ifndef CLIENTCONNECTRESPONSEHANDLER_H
#define CLIENTCONNECTRESPONSEHANDLER_H

#include <Session.h>
#include <ClientConnectResponse.h>

class ClientConnectResponseHandler
{
public:
    static void handler(ClientConnectResponse *response, Session *session);
};

#endif //CLIENTCONNECTRESPONSEHANDLER_H
