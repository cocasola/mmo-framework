//
// Created by Keegan Beaulieu on 2024-07-17.
//

#include "ReplicationChunk.h"

#include <NetworkMessage.h>

#include "Replication.h"

ReplicationChunk *ReplicationChunk::chunks;
int ReplicationChunk::grid_width = 100;
int ReplicationChunk::grid_height = 100;

ReplicationChunk::Ref ReplicationChunk::add_session(ClientSession *session)
{
    return {
        this,
        sessions.alloc(session)
    };
}

ReplicationChunk::Ref ReplicationChunk::add_entity(Entity *entity)
{
    return {
        this,
        entities.alloc(entity)
    };
}

void ReplicationChunk::replicate_entity_to_sessions(Entity *entity)
{
    for (auto session : sessions) {
        NetworkMessage::send_message<Replication>({ entity }, (Session *)session);
    }
}

void ReplicationChunk::replicate_entities_to_session(ClientSession *session)
{
    for (auto entity : entities) {
        NetworkMessage::send_message<Replication>({ entity }, (Session *)session);
    }
}

ReplicationChunk *ReplicationChunk::get_chunk(const Vector2i &coords)
{
    return &ReplicationChunk::chunks[coords.x*ReplicationChunk::grid_height + coords.y];
}

ReplicationChunk *ReplicationChunk::get_chunk(const Vector3f &world)
{
    Vector2i coords = get_chunk_coords(world);
    return get_chunk(coords);
}

Vector2i ReplicationChunk::get_chunk_coords(const Vector3f &world)
{
    Vector2i chunk = {
        (int)world.x,
        (int)world.z
    };

    chunk.x /= REPLICATION_CHUNK_SIZE;
    chunk.y /= REPLICATION_CHUNK_SIZE;

    chunk.x += grid_width/2;
    chunk.y += grid_height/2;

    return chunk;
}

void ReplicationChunk::init()
{
    chunks = (ReplicationChunk *)calloc(grid_width*grid_height, sizeof(ReplicationChunk));
}
