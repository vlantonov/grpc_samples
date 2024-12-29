cmake_minimum_required(VERSION 3.21)

project(conan_protobuf_sample)

add_subdirectory(proto)

add_executable(
    ${PROJECT_NAME}
    main.cpp
)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
        proto
)
