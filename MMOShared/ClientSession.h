//
// Created by Keegan Beaulieu on 2024-07-14.
//

#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <Entity.h>
#include <Session.h>

#include "ReplicationChunk.h"

class ClientSession : public Session
{
public:
    Entity *entity;
    ReplicationChunk::Ref replication_chunk_ref;

    ClientSession(const Endpoint &endpoint) : Session(endpoint) { }
};

#endif //CLIENTCONNECTION_H
