find_package(gRPC REQUIRED)
find_package(Protobuf REQUIRED)

set(PROTO_SRC
    helloworld.proto
)

add_library(
    proto
    STATIC
    ${PROTO_SRC}
)

target_link_libraries(
    proto
    PUBLIC
        gRPC::grpc++_reflection
        protobuf::libprotobuf
)

target_include_directories(
    proto
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

protobuf_generate(
    TARGET proto
    PROTOS ${PROTO_SRC}
)

protobuf_generate(
    TARGET proto
    PROTOS ${PROTO_SRC}
    LANGUAGE grpc
    GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
    PLUGIN protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
    PLUGIN_OPTIONS generate_mock_code=true
)
