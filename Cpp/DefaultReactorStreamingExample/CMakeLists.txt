cmake_minimum_required(VERSION 3.21)

project(route_guide_sample)

add_subdirectory(proto)

find_package(absl REQUIRED)

# Server
configure_file(route_guide_db.json . COPYONLY)

add_executable(
    route_guide_server
    helper.cpp
    route_guide_server.cpp
)

set_target_properties(
    route_guide_server
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_link_libraries(
    route_guide_server
    PRIVATE
        proto
        abseil::abseil
)

# Client
add_executable(
    route_guide_client
    helper.cpp
    route_guide_client.cpp
)

set_target_properties(
    route_guide_client
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_link_libraries(
    route_guide_client
    PRIVATE
        proto
        abseil::abseil
)