# gRPC health and reflection for Python

This sample shows how to register both the gRPC health service and the server reflection service, and then call the health check from a client.

## Install dependencies

```bash
python3 -m pip install grpcio grpcio-tools grpcio-health-checking grpcio-reflection
```

## Generate protobufs

```bash
python3 -m grpc_tools.protoc -I. --python_out=. --grpc_python_out=. ./helloworld.proto
```

## Start the server

```bash
python3 greeter_server.py
```

## Run the client

```bash
python3 greeter_client.py
```

The client verifies the gRPC health status for `helloworld.Greeter` and then calls the normal RPC.
