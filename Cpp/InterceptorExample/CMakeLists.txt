cmake_minimum_required(VERSION 3.21)

project(interceptor_example LANGUAGES CXX)

add_subdirectory(proto)

add_executable(
    interceptor_server
    server.cpp
)

set_target_properties(
    interceptor_server
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_link_libraries(
    interceptor_server
    PRIVATE
        proto
)

add_executable(
    interceptor_client
    client.cpp
)

set_target_properties(
    interceptor_client
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_link_libraries(
    interceptor_client
    PRIVATE
        proto
)
