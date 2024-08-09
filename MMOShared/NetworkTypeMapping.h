//
// Created by Keegan Beaulieu on 2024-07-14.
//

#ifndef NETWORKTYPEMAPPING_H
#define NETWORKTYPEMAPPING_H

#include <format>

class NetworkTypeMapping
{
public:
    enum NetworkId
    {
        CLIENT_CONNECT = 1,
        CLIENT_CONNECT_RESPONSE,
        REPLICATION,
        CPLAYER,
        CCAMERA,
        COMPONENT_MESSAGE,
        CMODEL3D
    };

    template <typename T>
    static inline int get_id();
};

#include "Component.h"
#include "CPlayer.h"
#include "Replication.h"
#include "ClientConnect.h"
#include "ClientConnectResponse.h"
#include "ComponentMessage.h"
#include "CCamera.h"
#include "CModel3d.h"

template <typename T>
inline int NetworkTypeMapping::get_id()
{
    if (std::is_same<T, ClientConnect>::value)
        return CLIENT_CONNECT;
    if (std::is_same<T, ClientConnectResponse>::value)
        return CLIENT_CONNECT_RESPONSE;
    if (std::is_same<T, Replication>::value)
        return REPLICATION;
    if (std::is_same<T, CPlayer>::value)
        return CPLAYER;
    if (std::is_same<T, ComponentMessage>::value)
        return COMPONENT_MESSAGE;
    if (std::is_same<T, CCamera>::value)
        return CCAMERA;
    if (std::is_same<T, CModel3d>::value)
        return CMODEL3D;
    else
        throw std::runtime_error(std::format("No network id for {}.", typeid(T).name()));
}

#endif //NETWORKTYPEMAPPING_H
