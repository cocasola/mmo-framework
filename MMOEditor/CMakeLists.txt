cmake_minimum_required(VERSION 3.28)
project(MMOEditor)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_FIND_FRAMEWORK LAST)
set(CMAKE_FIND_APPBUNDLE LAST)

find_package(OpenGL REQUIRED)

link_directories("/opt/homebrew/lib")
include_directories(${OPENGL_INCLUDE_DIRS} "/opt/homebrew/include/freetype2" "/opt/homebrew/include" "../MMOShared" "/Library/asio/include")

add_definitions(-D__EDITOR__)

add_library(MMOShared STATIC ../MMOShared/Serialization.cpp ../MMOShared/Network.cpp ../MMOShared/Session.cpp ../MMOShared/NetworkMessage.cpp ../MMOShared/Timer.cpp ../MMOShared/Entity.cpp
        ../MMOShared/Window.cpp
        ../MMOShared/Window.h
        ../MMOShared/Vector3f.cpp
        ../MMOShared/Vector3f.h
        ../MMOShared/Mesh.cpp
        ../MMOShared/Mesh.h
        ../MMOShared/Shader.cpp
        ../MMOShared/Shader.h
        ../MMOShared/Mat4x4f.cpp
        ../MMOShared/Mat4x4f.h
        ../MMOShared/CCamera.cpp
        ../MMOShared/CCamera.h
        ../MMOShared/UiContainer.cpp
        ../MMOShared/UiContainer.h
        ../MMOShared/Texture.cpp
        ../MMOShared/Texture.h
        ../MMOShared/Event.cpp
        ../MMOShared/Event.h
        ../MMOShared/Vector2i.cpp
        ../MMOShared/Vector2i.h
        ../MMOShared/Vector2f.cpp
        ../MMOShared/Vector2f.h
        ../MMOShared/RenderTarget.cpp
        ../MMOShared/RenderTarget.h
        ../MMOShared/Component.cpp
        ../MMOShared/Component.h
        ../MMOShared/CModel3d.cpp
        ../MMOShared/CModel3d.h
        ../MMOShared/Transform.cpp
        ../MMOShared/CPlayer.h
        ../MMOShared/CPlayer.cpp
        ../MMOShared/Components.cpp
        ../MMOShared/Components.h
        ../MMOShared/Replication.cpp
        ../MMOShared/Replication.h
        ../MMOShared/ComponentMessage.cpp
        ../MMOShared/ComponentMessage.h
        ../MMOShared/ReplicationChunk.cpp
        ../MMOShared/ReplicationChunk.h
        ../MMOShared/Material.cpp
        ../MMOShared/Material.h
        ../MMOShared/Colour.cpp
        ../MMOShared/Colour.h
        ../MMOShared/UiViewport.cpp
        ../MMOShared/UiViewport.h
        ../MMOShared/UiText.cpp
        ../MMOShared/UiText.h
        ../MMOShared/UiStyle.cpp
        ../MMOShared/UiStyle.h
        ../MMOShared/Context.cpp
        ../MMOShared/Context.h
        ../MMOShared/UiContextMenu.cpp
        ../MMOShared/UiContextMenu.h
        ../MMOShared/UiShadow.cpp
        ../MMOShared/UiShadow.h
        ../MMOShared/UiTextBox.cpp
        ../MMOShared/UiTextBox.h
        ../MMOShared/UiBorder.cpp
        ../MMOShared/UiBorder.h
        ../MMOShared/VirtualKey.cpp
        ../MMOShared/VirtualKey.h
        ../MMOShared/UiHLine.cpp
        ../MMOShared/UiHLine.h
        ../MMOShared/UiButton.cpp
        ../MMOShared/UiButton.h
        ../MMOShared/Vector4f.cpp
        ../MMOShared/Vector4f.h
        ../MMOShared/UiSearchBox.cpp
        ../MMOShared/UiSearchBox.h
        ../MMOShared/UiPopup.cpp
        ../MMOShared/UiPopup.h
)

add_executable(MMOEditor main.cpp
        Layout.cpp
        Layout.h
        Style.cpp
        Style.h
        ContextExplorer.cpp
        ContextExplorer.h
        ExplorerEntity.cpp
        ExplorerEntity.h
        ContextEditor.cpp
        ContextEditor.h
        EntityInspector.cpp
        EntityInspector.h
        Icon.cpp
        Icon.h
        ExposureEditor.cpp
        ExposureEditor.h
)

target_link_libraries(MMOEditor "-framework OpenGL" GLEW GLFW MMOShared freetype)
