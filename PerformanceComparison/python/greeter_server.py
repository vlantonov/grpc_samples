import logging
from concurrent import futures

import grpc

import helloworld_pb2
import helloworld_pb2_grpc


class Greeter(helloworld_pb2_grpc.GreeterServicer):
    def SayHello(self, request, context):
        return helloworld_pb2.HelloReply(message=f"Hello, {request.name}!")


def serve(port: int = 50070):
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=16))
    helloworld_pb2_grpc.add_GreeterServicer_to_server(Greeter(), server)
    server.add_insecure_port(f"[::]:{port}")
    server.start()
    logging.info("Python gRPC server listening on port %d", port)
    server.wait_for_termination()


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO, format="%(asctime)s %(levelname)s %(message)s")
    serve()
