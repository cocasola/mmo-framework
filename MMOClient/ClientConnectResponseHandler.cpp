//
// Created by Keegan Beaulieu on 2024-07-15.
//

#include "ClientConnectResponseHandler.h"

#include <Component.h>
#include <Entity.h>

#include "Player.h"

void ClientConnectResponseHandler::handler(ClientConnectResponse *response, Session *session)
{
    for (auto entity : Entity::entities) {
        if (entity.instance_id == response->player_instance_id) {
            Player::entity = &entity;
            break;
        }
    }
}
