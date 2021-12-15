# Example of simple C++ gRPC service with Docker build

This sample demonstrates how to create a basic gRPC service in C++ and build it
it using GCC and CMake in a Docker container. The grpc dependencies are installed
using the default package manager.

## Build

```sh
docker build -t simple_cpp_grpc .
```

## Run Client

```sh
docker run --rm -it --network=host --entrypoint /bin/bash simple_cpp_grpc start_client.sh
```

## Run Server

```sh
docker run --rm -it --network=host simple_cpp_grpc
```

## References

* https://github.com/npclaudiu/grpc-cpp-docker
* https://github.com/IvanSafonov/grpc-cmake-example
* https://github.com/googlecloudrobotics/core/tree/master/docs/how-to/examples/greeter-service
