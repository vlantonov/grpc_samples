# C++ bidirectional streaming with backpressure demo

This sample demonstrates a slow server handling a bidirectional gRPC stream. The client pushes messages quickly while the server intentionally sleeps between replies to illustrate backpressure.

## Build

```bash
conan install . --output-folder=build --build=missing
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=build/build/Release/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Start the server

```bash
./build/bidi_backpressure_server --port 50052
```

## Run the client

```bash
./build/bidi_backpressure_client --host localhost --port 50052 --count 10 --send-delay-ms 200
```

The server deliberately waits 750 ms before replying to each incoming request, while the client sends at 200 ms intervals. This difference makes the stream naturally back up and shows the effect of backpressure in practice.
