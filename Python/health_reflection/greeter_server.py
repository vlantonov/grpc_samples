import logging
from concurrent import futures

import grpc
import helloworld_pb2
import helloworld_pb2_grpc
from grpc_health.v1 import health, health_pb2, health_pb2_grpc
from grpc_reflection.v1alpha import reflection


class Greeter(helloworld_pb2_grpc.GreeterServicer):
    def SayHello(self, request, context):
        return helloworld_pb2.HelloReply(message=f"Hello, {request.name}!")


def serve(port: int = 50053):
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    helloworld_pb2_grpc.add_GreeterServicer_to_server(Greeter(), server)

    health_servicer = health.HealthServicer()
    health_pb2_grpc.add_HealthServicer_to_server(health_servicer, server)
    health_servicer.set("helloworld.Greeter", health_pb2.HealthCheckResponse.SERVING)
    health_servicer.set("", health_pb2.HealthCheckResponse.SERVING)

    service_names = (
        helloworld_pb2.DESCRIPTOR.services_by_name["Greeter"].full_name,
        health_pb2.DESCRIPTOR.services_by_name["Health"].full_name,
        reflection.SERVICE_NAME,
    )
    reflection.enable_server_reflection(service_names, server)

    server.add_insecure_port(f"[::]:{port}")
    server.start()
    logging.info("Health and reflection server listening on port %d", port)
    server.wait_for_termination()


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO, format="%(asctime)s %(levelname)s %(message)s")
    serve()
