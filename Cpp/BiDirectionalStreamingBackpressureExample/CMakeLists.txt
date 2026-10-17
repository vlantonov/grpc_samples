cmake_minimum_required(VERSION 3.21)

project(bidi_backpressure_example LANGUAGES CXX)

add_subdirectory(proto)

add_executable(
    bidi_backpressure_server
    server.cpp
)

set_target_properties(
    bidi_backpressure_server
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_link_libraries(
    bidi_backpressure_server
    PRIVATE
        proto
)

add_executable(
    bidi_backpressure_client
    client.cpp
)

set_target_properties(
    bidi_backpressure_client
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_link_libraries(
    bidi_backpressure_client
    PRIVATE
        proto
)
