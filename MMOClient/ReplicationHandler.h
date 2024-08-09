//
// Created by Keegan Beaulieu on 2024-07-18.
//

#ifndef REPLICATIONHANDLER_H
#define REPLICATIONHANDLER_H

#include <Replication.h>
#include <Session.h>

class ReplicationHandler
{
public:
    static void handler(Replication *replication, Session *session);
};

#endif //REPLICATIONHANDLER_H
