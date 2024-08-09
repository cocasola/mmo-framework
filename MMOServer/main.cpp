#include <Component.h>
#include <Entity.h>
#include <Timer.h>
#include <Network.h>
#include <Components.h>

#include "NetworkMessageHandlers.h"

#define TICK_RATE (1.0f/30.0f)

int main()
{
    Network::init_as_server(30000);
    NetworkMessageHandlers::register_handlers();
    ReplicationChunk::init();
    Components::register_all();

    Timer timer;

    for (;;) {
        timer.reset();

        Network::read_packets();

        float delta = timer.read_seconds();
        Timer::wait_seconds(TICK_RATE - delta);
    }

    Network::cleanup();
    Entity::cleanup();

    return 0;
}
