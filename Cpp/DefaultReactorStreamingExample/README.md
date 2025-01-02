# Streaming RPC examples using default Reactors
* Using `build` folder

## Callback-based async API samples
* Unary RPC
* Server-side streaming RPC
* Client-side streaming RPC
* Bidirectional streaming RPC

## Debug build
```
conan install .. --build=missing -pr:b=default -s build_type=Debug
cmake .. --preset conan-debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cd Debug
make
```

## Release build
```
conan install .. --build=missing -pr:b=default -s build_type=Release
cmake .. --preset conan-release
cd Release
make
```

## References
* [Asynchronous Callback API Tutorial](https://grpc.io/docs/languages/cpp/callback/)
* [Unary RPC](https://grpc.io/docs/languages/cpp/callback/#unary-rpc)
* [Server-side streaming RPC](https://grpc.io/docs/languages/cpp/callback/#server-side-streaming-rpc)
* [Client-side streaming RPC](https://grpc.io/docs/languages/cpp/callback/#client-side-streaming-rpc)
* [Bidirectional streaming RPC](https://grpc.io/docs/languages/cpp/callback/#bidirectional-streaming-rpc)
* <https://github.com/grpc/grpc/tree/v1.66.0/examples/cpp/route_guide>
