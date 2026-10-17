cmake_minimum_required(VERSION 3.21)

project(perf_compare_cpp LANGUAGES CXX)

add_subdirectory(proto)

add_executable(perf_server server.cpp)
add_executable(perf_client client.cpp)

set_target_properties(perf_server perf_client PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)

target_link_libraries(perf_server PRIVATE proto)
target_link_libraries(perf_client PRIVATE proto)
