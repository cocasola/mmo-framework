//
// Created by Keegan Beaulieu on 2024-07-17.
//

#ifndef REPLICATION_H
#define REPLICATION_H

class Entity;

struct Replication
{
    Entity *entity;

    static void serialize(unsigned char *&bytes, unsigned char *end, Replication *replication);
    static void deserialize(unsigned char *&bytes, unsigned char *end, Replication *replication);
};

#endif //REPLICATION_H
