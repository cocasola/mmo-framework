//
// Created by Keegan Beaulieu on 2024-07-13.
//

#include "NetworkMessageHandlers.h"

#include "ClientConnectHandler.h"

void NetworkMessageHandlers::register_handlers()
{
    NetworkMessage::register_handler<ClientConnect>(&ClientConnectHandler::handler);
}
