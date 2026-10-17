cmake_minimum_required(VERSION 3.21)

project(health_reflection_example LANGUAGES CXX)

add_subdirectory(proto)

add_executable(
    health_reflection_server
    server.cpp
)

set_target_properties(
    health_reflection_server
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_link_libraries(
    health_reflection_server
    PRIVATE
        proto
)

add_executable(
    health_reflection_client
    client.cpp
)

set_target_properties(
    health_reflection_client
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_link_libraries(
    health_reflection_client
    PRIVATE
        proto
)
