//
// Created by Keegan Beaulieu on 2024-07-13.
//

#ifndef NETWORK_H
#define NETWORK_H

#include <asio.hpp>
#include <List.h>

typedef asio::ip::udp::endpoint Endpoint;
typedef asio::ip::address Address;

struct EndpointHasher
{
    std::size_t operator()(const Endpoint& endpoint) const
    {
        size_t hash = 0;
        Address address = endpoint.address();

        if (address.is_v4()) {
            hash += endpoint.address().to_v4().to_ulong();
        } else {
            auto bytes = endpoint.address().to_v6().to_bytes();

            for (auto i : bytes) {
                hash += bytes[i];
            }
        }

        hash += endpoint.port();

        return hash;
    }
};

struct Packet
{
    size_t size;
    unsigned char *bytes;
    Endpoint endpoint;
};

class Network
{
public:
    static asio::ip::udp::endpoint endpoint;

    static void init_as_server(int port);
    static void init_as_client(std::string hostname, int port);
    static void cleanup();

    static void read_packets();
    static void send_packet(Packet &packet);

private:
    static asio::io_context io_context;
    static asio::ip::udp::socket *socket;
};

#endif //NETWORK_H
