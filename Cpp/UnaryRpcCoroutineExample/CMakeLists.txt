cmake_minimum_required(VERSION 3.21)

project(unary_coroutine_sample)

add_subdirectory(proto)

# Server
add_executable(
    unary_coroutine_server
    server.cpp
)

set_target_properties(
    unary_coroutine_server
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_link_libraries(
    unary_coroutine_server
    PRIVATE
        proto
)

# Client
add_executable(
    unary_coroutine_client
    client.cpp
)

set_target_properties(
    unary_coroutine_client
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_link_libraries(
    unary_coroutine_client
    PRIVATE
        proto
)
