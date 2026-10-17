import logging
from contextvars import ContextVar

import grpc
import helloworld_pb2
import helloworld_pb2_grpc


TRACEPARENT = ContextVar("traceparent", default="00-4bf92f3577b34da6a3ce929d0e0e4736-00f067aa0ba902b7-01")


class ClientAuthTraceInterceptor(grpc.UnaryUnaryClientInterceptor):
    def intercept_unary_unary(self, continuation, client_call_details, request):
        metadata = list(client_call_details.metadata or [])
        metadata = [item for item in metadata if not item[0].lower() == "authorization" and not item[0].lower() == "traceparent"]

        metadata.append(("authorization", "Bearer demo-token"))
        metadata.append(("traceparent", TRACEPARENT.get()))

        logging.info("outgoing metadata: %s", dict(metadata))
        call_details = client_call_details._replace(metadata=metadata)
        return continuation(call_details, request)


def run():
    channel = grpc.insecure_channel("localhost:50054")
    intercept_channel = grpc.intercept_channel(channel, ClientAuthTraceInterceptor())
    stub = helloworld_pb2_grpc.GreeterStub(intercept_channel)
    response = stub.SayHello(helloworld_pb2.HelloRequest(name="world"))
    print(f"Greeter client received: {response.message}")


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO, format="%(asctime)s %(levelname)s %(message)s")
    run()
