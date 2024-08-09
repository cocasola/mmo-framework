#include <iostream>

#include <Network.h>
#include <Timer.h>
#include <NetworkMessage.h>

#include <Context.h>
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
#include <UiContextMenu.h>
#include <UiStyle.h>
#include <UiText.h>
#include <UiViewport.h>

#include "ContextEditor.h"
#include "Icon.h"
#include "Layout.h"
#include "Style.h"

#define TICK_RATE (1/60.0f)

int main(int argc, char *argv[])
{
    Window::init({ .title = "MMO Editor", .width = 1200, .height = 700, .x = 0, .y = 0 });

    Context::init();
    Shader::init();
    Material::init();
    Mesh::init();
    UiStyle::init();
    UiContainer::init();
    UiViewport::init();
    UiText::init();
    Icon::init();
    Components::register_all();

    Style::register_styles();
    Layout::init();

    Timer timer;

    while (Window::poll() != true) {
        UiContainer::fire_deferred();

        timer.reset();

        UiViewport::clear_all();
        Component::tick_components();
        if (ContextEditor::current)
            ContextEditor::current->draw_gizmos();
        RenderTarget::unbind();

        UiContainer::render_tree();

        Window::swap();

        float delta = timer.read_seconds();
        Timer::wait_seconds(TICK_RATE - delta);
    }

    Layout::cleanup();

    UiText::cleanup();
    UiContainer::cleanup();
    UiStyle::cleanup();
    Mesh::cleanup();
    Shader::cleanup();
    Material::cleanup();
    Context::cleanup();

    Window::cleanup();

    return 0;
}
