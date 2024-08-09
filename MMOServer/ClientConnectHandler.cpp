//
// Created by Keegan Beaulieu on 2024-07-14.
//

#include "ClientConnectHandler.h"

#include <iostream>

#include <ClientConnectResponse.h>

void ClientConnectHandler::handler(ClientConnect *, Session *session)
{
    ClientSession *client_session = dynamic_cast<ClientSession *>(session);
    if (!client_session)
        return;

    Vector2i coords = ReplicationChunk::get_chunk_coords(client_session->entity->transform->position);

    for (int x = coords.x - 1; x < coords.x + 1; ++x) {
        for (int y = coords.y - 1; y < coords.y + 1; ++y) {
            ReplicationChunk *chunk = ReplicationChunk::get_chunk(Vector2i(x, y));
            chunk->replicate_entities_to_session(client_session);
        }
    }

    NetworkMessage::send_message<ClientConnectResponse>({ client_session->entity->instance_id }, session);
}
