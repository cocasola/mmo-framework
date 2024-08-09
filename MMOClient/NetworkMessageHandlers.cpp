//
// Created by Keegan Beaulieu on 2024-07-13.
//

#include "NetworkMessageHandlers.h"

#include <NetworkMessage.h>

#include "ClientConnectResponseHandler.h"
#include "ComponentMessageHandler.h"
#include "ReplicationHandler.h"

void NetworkMessageHandlers::register_handlers()
{
    NetworkMessage::register_handler(&ClientConnectResponseHandler::handler);
    NetworkMessage::register_handler(&ReplicationHandler::handler);
    NetworkMessage::register_handler(&ComponentMessageHandler::handler);
}
