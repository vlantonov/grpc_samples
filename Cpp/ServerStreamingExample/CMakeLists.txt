cmake_minimum_required(VERSION 3.21)

project(server_streaming_sample)

add_subdirectory(proto)

# Server
add_executable(
    server_streaming_server
    server.cpp
)

set_target_properties(
    server_streaming_server
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_link_libraries(
    server_streaming_server
    PRIVATE
        proto
)

# Client
add_executable(
    server_streaming_client
    client.cpp
)

set_target_properties(
    server_streaming_client
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_link_libraries(
    server_streaming_client
    PRIVATE
        proto
)
