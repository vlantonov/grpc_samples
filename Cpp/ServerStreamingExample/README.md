# Server-side streaming RPC example

* Callback-based async API sample for server-side streaming RPC
* The client sends a request to the server
* The server will return a sequence of responses to the client, each of which is sent in a separate message
* Client reactor encapsulates all the operations needed to process an RPC.
* Override the methods of the server Reactor to implement the RPC.
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
* [Server-side streaming RPC](https://grpc.io/docs/languages/cpp/callback/#server-side-streaming-rpc)
* [Exploring how to implement the server using gRPC async callbacks](https://lastviking.eu/fun_with_gRPC_and_C++/callback-server.html)
* <https://github.com/jgaa/fun-with-gRPC/>
