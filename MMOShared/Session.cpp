//
// Created by Keegan Beaulieu on 2024-07-14.
//

#include "Session.h"

#include <iostream>

#include "NetworkMessage.h"

std::unordered_map<Endpoint, Session *, EndpointHasher> Session::session_map;
List<Session> Session::sessions;

void Session::match_packet(Packet &packet)
{
    try {
        Session *session = session_map.at(packet.endpoint);
        NetworkMessage::handle_message(packet, session);
    } catch (...) {

    }
}
