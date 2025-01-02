# Unary RPC example

* Callback-based async API sample for unary RPC
* Custom reactor to handle specific actions such as RPC cancellation or run an action asynchronously when the RPC is done.
* Useless - just focus on the coroutine support code
* Using `build` folder

## Why use coroutines for a unary RPC?
* Opt in to the more scalable reactor implementation
* Allows the use of other async coroutine code in the implementation

## Coroutine notes
* Specialise [std::coroutine_traits](https://en.cppreference.com/w/cpp/coroutine/coroutine_traits) for unary RPCs
* Implement a basic gRPC `promise_type` - mostly boilerplate
* Add support for returning a status using `co_return`
* Simple `Reactor` side of the code.

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
* [Custom Unary Reactor](https://grpc.io/docs/languages/cpp/callback/#custom-unary-reactor)
* [Introduction to C++ Coroutines and gRPC and How to Write Coroutine Support Code - Jonathan Storey](https://www.youtube.com/watch?v=deUbQodyaC4)
* [Coroutines and gRPC - Online C++ Conference](https://cpponline.uk/wp-content/uploads/2024/03/coroutines_and_grpc_cpp_online_2024_slides.pdf)
