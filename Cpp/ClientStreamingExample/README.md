# Client-side streaming RPC example

* Callback-based async API sample for client-side streaming RPC
* The client sends a sequence of requests to the server
* The server will return a reply after the client has finished sending the requests
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
* [Client-side streaming RPC](https://grpc.io/docs/languages/cpp/callback/#client-side-streaming-rpc)
