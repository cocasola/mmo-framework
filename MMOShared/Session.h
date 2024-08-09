//
// Created by Keegan Beaulieu on 2024-07-14.
//

#ifndef SESSION_H
#define SESSION_H

#include <Network.h>

class Session
{
public:
    Endpoint endpoint;

    Session(const Endpoint &endpoint): endpoint(endpoint) { };
    virtual ~Session() = default;

    static void match_packet(Packet &packet);

    template <typename T>
    static T *register_session(const Endpoint &endpoint);

private:
    static std::unordered_map<Endpoint, Session *, EndpointHasher> session_map;
    static List<Session> sessions;
};

template <typename T>
T *Session::register_session(const Endpoint &endpoint)
{
    T session(endpoint);
    T *ptr = ((List<T> *)&sessions)->alloc(session);

    session_map[endpoint] = (Session *)ptr;

    return ptr;
}

#endif //SESSION_H
