# Notes

## Prerequisite
`pip3 install --upgrade protobuf`

## Generate buffers
`python3 -m grpc_tools.protoc -I. --python_out=. --grpc_python_out=. ./helloworld.proto`

## Resulting buffers
* `helloworld_pb2.py`
* `helloworld_pb2_grpc.py`

## Server
`python3 greeter_server.py`

## Client
`python3 greeter_client.py`

## Links
* <https://grpc.io/docs/languages/python/basics/>
