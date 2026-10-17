cmake_minimum_required(VERSION 3.21)

project(tls_example LANGUAGES CXX)

add_subdirectory(proto)

add_executable(
    tls_server
    server.cpp
)

set_target_properties(
    tls_server
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_link_libraries(
    tls_server
    PRIVATE
        proto
)

add_executable(
    tls_client
    client.cpp
)

set_target_properties(
    tls_client
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_link_libraries(
    tls_client
    PRIVATE
        proto
)
