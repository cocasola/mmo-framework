//
// Created by Keegan Beaulieu on 2024-07-14.
//

#ifndef CLIENTCONNECTRESPONSE_H
#define CLIENTCONNECTRESPONSE_H

#include <vector>

#include "Serialization.h"

struct ClientConnectResponse
{
    int player_instance_id;

    SERIALIZABLE({
        PRIMITIVE(ClientConnectResponse, player_instance_id)
    })
};

#endif //CLIENTCONNECTRESPONSE_H
