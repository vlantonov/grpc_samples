# gRPC middleware / interceptor sample (Python)

This sample demonstrates middleware-style behavior using gRPC interceptors:

- client interceptor adds `Authorization` and `traceparent` metadata
- server interceptor logs the request metadata
- server-side auth check rejects missing or invalid bearer tokens
- OpenTelemetry-style trace propagation uses the standard `traceparent` header

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
