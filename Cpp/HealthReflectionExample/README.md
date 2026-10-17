# C++ gRPC health and reflection example

This sample registers the standard gRPC health service and server reflection, and demonstrates checking the health status from a client before invoking the actual RPC.

## Build

```bash
conan install . --output-folder=build --build=missing
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=build/build/Release/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Start the server

```bash
./build/health_reflection_server --port 50053
```

## Run the client

```bash
./build/health_reflection_client --host localhost --port 50053
```

The server exposes the standard health check service and enables server reflection so tools such as `grpcurl` can inspect the loaded services.
