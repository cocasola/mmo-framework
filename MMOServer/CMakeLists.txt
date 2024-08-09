cmake_minimum_required(VERSION 3.28)
project(MMOServer)

set(CMAKE_CXX_STANDARD 20)

include_directories("/opt/homebrew/include" "../MMOShared" "/Library/asio/include")

add_definitions(-D__SERVER__)

add_library(MMOShared STATIC
        ../MMOShared/Timer.cpp
        ../MMOShared/Timer.h
        ../MMOShared/NetworkMessage.cpp
        ../MMOShared/NetworkMessage.h
        ../MMOShared/Network.cpp
        ../MMOShared/Network.h
        ../MMOShared/Serialization.cpp
        ../MMOShared/Serialization.h
        ../MMOShared/ClientConnectResponse.h
        ../MMOShared/Session.cpp
        ../MMOShared/Session.h
        ../MMOShared/NetworkTypeMapping.h
        ../MMOShared/ClientConnect.h
        ../MMOShared/Entity.cpp
        ../MMOShared/Entity.h
        ../MMOShared/Transform.cpp
        ../MMOShared/Component.cpp
        ../MMOShared/ClientSession.cpp
        ../MMOShared/ClientSession.h
        ../MMOShared/Replication.h
        ../MMOShared/Replication.cpp
        ../MMOShared/CPlayer.cpp
        ../MMOShared/CPlayer.h
        ../MMOShared/Components.cpp
        ../MMOShared/Components.h
        ../MMOShared/ComponentMessage.cpp
        ../MMOShared/ComponentMessage.h
)

add_executable(MMOServer main.cpp
        NetworkMessageHandlers.cpp
        NetworkMessageHandlers.h
        ClientConnectHandler.cpp
        ClientConnectHandler.h
        ../MMOShared/ReplicationChunk.cpp
        ../MMOShared/ReplicationChunk.h
)

target_link_libraries(MMOServer MMOShared)