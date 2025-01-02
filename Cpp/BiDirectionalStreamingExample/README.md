# Bidirectional streaming RPC example

* Callback-based async API sample for bidirectional streaming RPC
* the client sends a sequence of requests to the server. 
* Each time a request is sent, the server will return a sequence of replies at the same request that have been sent by all the clients before.
* Clients can share information with each other
* Using `build` folder

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
* [Bidirectional streaming RPC](https://grpc.io/docs/languages/cpp/callback/#bidirectional-streaming-rpc)
