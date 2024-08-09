//
// Created by Keegan Beaulieu on 2024-07-17.
//

#ifndef CPLAYER_H
#define CPLAYER_H

#include "Component.h"
#include "Serialization.h"

class ClientSession;
class Entity;

class CPlayer : public Component
{
public:
#ifdef __SERVER__
    ClientSession *session;
#endif

    Entity *entity;
    int x;

    static void register_type();
    static void on_add(CPlayer *player);
};

#endif //CPLAYER_H
