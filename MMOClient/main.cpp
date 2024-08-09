#include <iostream>

#include <Network.h>
#include <Timer.h>
#include <NetworkMessage.h>

#include <CCamera.h>
#include <Component.h>
#include <Components.h>
#include <Mesh.h>
#include <Shader.h>
#include <UiContainer.h>
#include <Window.h>

#include <CPlayer.h>
#include <Material.h>
#include <RenderTarget.h>

#include "NetworkMessageHandlers.h"

#define TICK_RATE (1/60.0f)

int main(int argc, char *argv[])
{
    Network::init_as_client("127.0.0.1", 30000);
    NetworkMessageHandlers::register_handlers();

    Window::init({ .title = "MMO Client" });

    Shader::init();
    Material::init();
    Mesh::init();
    UiContainer::init();

    Components::register_all();

    // Session *session = Session::register_session<Session>(Network::endpoint);
    //
    // unsigned char bytes[] = { 200 };
    //
    // Packet packet{
    //     1,
    //     bytes,
    //     Network::endpoint
    // };
    //
    // Network::send_packet(packet);
    // Timer::wait_seconds(0.1f);
    // NetworkMessage::send_message<ClientConnect>({}, session);

    RenderTarget *target = RenderTarget::create({
        .width = UiContainer::root_container->calculated_size.x,
        .height = UiContainer::root_container->calculated_size.y
    });

    UiContainer::root_container->texture = target->texture;

    Timer timer;

    while (Window::poll() != true) {
        timer.reset();

        Network::read_packets();

        UiContainer::render_tree();
        Window::swap();

        float delta = timer.read_seconds();
        Timer::wait_seconds(TICK_RATE - delta);
    }

    UiContainer::cleanup();
    Mesh::cleanup();
    Shader::cleanup();
    Material::cleanup();
    Window::cleanup();

    return 0;
}