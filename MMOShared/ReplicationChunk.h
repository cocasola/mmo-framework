//
// Created by Keegan Beaulieu on 2024-07-17.
//

#ifndef REPLICATION_CHUNK_H
#define REPLICATION_CHUNK_H

#define REPLICATION_CHUNK_SIZE 100

#include "List.h"
#include "Vector2i.h"
#include "Vector3f.h"

class Entity;
class ClientSession;

class ReplicationChunk
{
public:
    struct Ref
    {
        ReplicationChunk *chunk;
        void *location;
    };

    List<ClientSession *> sessions;
    List<Entity *> entities;

    static ReplicationChunk *chunks;
    static int grid_width;
    static int grid_height;

    Ref add_session(ClientSession *session);
    Ref add_entity(Entity *entity);

    void replicate_entity_to_sessions(Entity *entity);
    void replicate_entities_to_session(ClientSession *session);

    static Vector2i get_chunk_coords(const Vector3f &world);
    static ReplicationChunk *get_chunk(const Vector2i &coords);
    static ReplicationChunk *get_chunk(const Vector3f &world);

    static void init();
};

#endif //REPLICATION_CHUNK_H
