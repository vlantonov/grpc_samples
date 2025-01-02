# Unary RPC example

* Callback-based async API sample for unary RPC
* Custom reactor to handle specific actions such as RPC cancellation or run an action asynchronously when the RPC is done.
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
* [Unary RPC](https://grpc.io/docs/languages/cpp/callback/#unary-rpc)
* [Custom Unary Reactor](https://grpc.io/docs/languages/cpp/callback/#custom-unary-reactor)
