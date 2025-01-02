# Server-side streaming RPC coroutines example

* Callback-based async API sample for server-side streaming RPC
* The client sends a request to the server
* The server will return a sequence of responses to the client, each of which is sent in a separate message
* Client reactor encapsulates all the operations needed to process an RPC.
* Override the methods of the server Reactor to implement the RPC.
* Using `build` folder

## Coroutine notes
* Access to the Reactor is needed to write the response
* The Reactor lives in the `promise_type`
* In order for `co_await send(response)` to work, the result of `send(response)` needs to be awaitable
* The `awaitable` is transformed to `awaiter`
* The `awaiter` suspends the coroutine, starts writing the response and schedules coroutine resume on write completion.
* Reactor is called in the `awaiter_suspend` to start writing
* The `coroutine_handle` is stored in the Reactor to know what to resume when write completes
* Reactor destroys the `coroutine_handle` in case of failure

## Coroutine aware functions
* Start the operation
* Return an `awaitable` directly
* Calling `co_await awaitable` suspends the coroutine until the operation is complete
* If the gRPC call is cancelled it is desired to stop immediately

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
* [Introduction to C++ Coroutines and gRPC and How to Write Coroutine Support Code - Jonathan Storey](https://www.youtube.com/watch?v=deUbQodyaC4)
* [Coroutines and gRPC - Online C++ Conference](https://cpponline.uk/wp-content/uploads/2024/03/coroutines_and_grpc_cpp_online_2024_slides.pdf)
* <https://github.com/npuichigo/agrpc/>