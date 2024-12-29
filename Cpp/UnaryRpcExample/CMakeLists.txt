cmake_minimum_required(VERSION 3.21)

project(callback_service_sample)

add_subdirectory(proto)

# Server
add_executable(
    callback_service_server
    server.cpp
)

set_target_properties(
    callback_service_server
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_link_libraries(
    callback_service_server
    PRIVATE
        proto
)

# Client
add_executable(
    callback_service_client
    client.cpp
)

set_target_properties(
    callback_service_client
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_link_libraries(
    callback_service_client
    PRIVATE
        proto
)
