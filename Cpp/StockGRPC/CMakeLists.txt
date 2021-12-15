cmake_minimum_required (VERSION 3.7.2)

project(grpc_hello_world LANGUAGES CXX)

# Path to GRPC
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/build/cmake/")

find_package(Protobuf REQUIRED)
find_package(GRPC REQUIRED)

set(Protobuf_IMPORT_DIRS "${CMAKE_SOURCE_DIR}/")

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS helloworld.proto)
PROTOBUF_GENERATE_GRPC_CPP(GRPC_SRCS GRPC_HDRS helloworld.proto)

# Server
add_executable(
  greeter_server
  ${PROTO_HDRS}
  ${PROTO_SRCS}
  ${GRPC_SRCS}
  ${GRPC_HDRS}
  greeter_server.cc
)

set_target_properties(
  greeter_server
  PROPERTIES
      CXX_STANDARD 14
      CXX_STANDARD_REQUIRED YES
      CXX_EXTENSIONS NO
)

target_compile_options(
  greeter_server
  PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

target_include_directories(
  greeter_server
  PRIVATE
    ${PROTOBUF_INCLUDE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)
 
target_link_libraries(
  greeter_server
  PRIVATE
    grpc++
    grpc++_reflection
    protobuf
)

# Client
add_executable(
  greeter_client
  ${PROTO_HDRS}
  ${PROTO_SRCS}
  ${GRPC_SRCS}
  ${GRPC_HDRS}
  greeter_client.cc
)

set_target_properties(
  greeter_client
  PROPERTIES
      CXX_STANDARD 14
      CXX_STANDARD_REQUIRED YES
      CXX_EXTENSIONS NO
)

target_compile_options(
  greeter_client
  PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

target_include_directories(
  greeter_client
  PRIVATE
    ${PROTOBUF_INCLUDE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)
 
target_link_libraries(
  greeter_client
  PRIVATE
    grpc++
    grpc++_reflection
    protobuf
)

install(
  TARGETS
    greeter_client
    greeter_server 
  DESTINATION
    /usr/bin
)
