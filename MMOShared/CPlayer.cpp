//
// Created by Keegan Beaulieu on 2024-07-17.
//

#include "NetworkTypeMapping.h"
#include "CPlayer.h"

#include <iostream>
#include <Timer.h>

#include "Component.h"

void CPlayer::on_add(CPlayer *player)
{

}

void CPlayer::register_type()
{
    Component::register_type<CPlayer>({
        .expose = {
            EXPOSE(REPLICATED, CPlayer, x)
        },
        .on_add = &on_add,
        .name = "Player"
    });

    auto a = &typeid(CPlayer);
    auto b = &typeid(CPlayer);

    assert(a == b);
}
