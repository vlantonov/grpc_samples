# C++ interceptor / middleware sample

This sample demonstrates a middleware-style gRPC setup using client and server interceptors.

- client interceptor adds `Authorization` and `traceparent` metadata
- server interceptor logs the incoming metadata
- service-level auth rejects missing or invalid bearer tokens
- trace propagation follows the OpenTelemetry `traceparent` convention

## Build

```bash
conan install . --output-folder=build --build=missing
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=build/build/Release/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Start the server

```bash
./build/interceptor_server --port 50054
```

## Run the client

```bash
./build/interceptor_client --host localhost --port 50054
```
