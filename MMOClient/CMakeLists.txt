cmake_minimum_required(VERSION 3.28)
project(MMOClient)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_FIND_FRAMEWORK LAST)
set(CMAKE_FIND_APPBUNDLE LAST)

find_package(OpenGL REQUIRED)

link_directories("/opt/homebrew/lib")
include_directories(${OPENGL_INCLUDE_DIRS} "/opt/homebrew/include" "../MMOShared" "/Library/asio/include")

add_definitions(-D__CLIENT__)

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
)

add_executable(MMOClient main.cpp
        ClientConnectResponseHandler.cpp
        ClientConnectResponseHandler.h
        NetworkMessageHandlers.cpp
        ReplicationHandler.cpp
        ReplicationHandler.h
        Player.cpp
        Player.h
        ComponentMessageHandler.cpp
        ComponentMessageHandler.h
)

target_link_libraries(MMOClient "-framework OpenGL" GLEW GLFW MMOShared)