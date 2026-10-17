# Bidirectional streaming with backpressure demo

This sample shows a gRPC bidirectional stream where the server intentionally processes each request slowly to demonstrate how gRPC backpressure naturally appears when the downstream consumer cannot keep up.

## Generate buffers

```bash
python3 -m grpc_tools.protoc -I. --python_out=. --grpc_python_out=. ./chat.proto
```

## Start the server

```bash
python3 chat_server.py
```

## Run the client

```bash
python3 chat_client.py
```

The client sends a burst of messages with a short delay between messages. The server sleeps for 0.75s before replying to each request, which means the stream naturally queues work on the client and shows backpressure behavior in practice.
