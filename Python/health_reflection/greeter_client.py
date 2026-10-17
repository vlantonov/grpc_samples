import grpc
import helloworld_pb2
import helloworld_pb2_grpc
from grpc_health.v1 import health_pb2, health_pb2_grpc


def run():
    with grpc.insecure_channel("localhost:50053") as channel:
        health_stub = health_pb2_grpc.HealthStub(channel)
        response = health_stub.Check(
            health_pb2.HealthCheckRequest(service="helloworld.Greeter")
        )
        print(f"Health status: {response.status}")

        greeter_stub = helloworld_pb2_grpc.GreeterStub(channel)
        reply = greeter_stub.SayHello(helloworld_pb2.HelloRequest(name="world"))
        print(f"Greeter client received: {reply.message}")


if __name__ == "__main__":
    run()
