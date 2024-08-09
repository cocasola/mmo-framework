//
// Created by Keegan Beaulieu on 2024-07-13.
//

#include "Network.h"

#include <iostream>

#include "Session.h"
#include "Component.h"
#include "ClientSession.h"
#include "ReplicationChunk.h"

#define BUFFER_SIZE 2048

asio::io_context Network::io_context;
asio::ip::udp::socket *Network::socket;
asio::ip::udp::endpoint Network::endpoint;

void Network::init_as_server(int port)
{
    socket = new asio::ip::udp::socket(
        io_context,
        asio::ip::udp::endpoint(asio::ip::udp::v4(), port)
    );
}

void Network::init_as_client(std::string hostname, int port)
{
    asio::ip::udp::resolver resolver(io_context);
    endpoint = *resolver.resolve(asio::ip::udp::v4(), hostname, std::to_string(port)).begin();
    socket = new asio::ip::udp::socket(io_context);
    socket->open(asio::ip::udp::v4());
}

void Network::cleanup()
{
    delete socket;
}

void Network::read_packets()
{
    size_t size = socket->available();

    while (size > 0) {
        unsigned char *bytes = (unsigned char *)malloc(size);

        asio::ip::udp::endpoint endpoint;
        socket->receive_from(asio::buffer(bytes, size), endpoint);

        Packet packet{
            size,
            bytes,
            endpoint
        };

#ifdef __SERVER__
        if (bytes[0] == 200) {
            ClientSession *session = Session::register_session<ClientSession>(endpoint);

            session->entity = Entity::create(std::to_string(endpoint.port()));
            CPlayer *player = session->entity->add_component<CPlayer>();

            Vector2i coords = ReplicationChunk::get_chunk_coords(session->entity->transform->position);
            ReplicationChunk *chunk = ReplicationChunk::get_chunk(coords);

            for (int x = coords.x - 1; x < coords.x + 1; ++x) {
                for (int y = coords.y - 1; y < coords.y + 1; ++y) {
                    ReplicationChunk *chunk = ReplicationChunk::get_chunk(Vector2i(x, y));
                    chunk->replicate_entity_to_sessions(session->entity);
                }
            }

            session->entity->replication_chunk_ref = chunk->add_entity(session->entity);
            session->replication_chunk_ref = chunk->add_session(session);

            return;
        }
#endif

        Session::match_packet(packet);
        free(packet.bytes);

        size = socket->available();
    }
}

void Network::send_packet(Packet &packet)
{
    socket->send_to(asio::buffer(packet.bytes, packet.size), packet.endpoint);
}
