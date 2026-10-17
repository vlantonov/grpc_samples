import logging
from concurrent import futures

import grpc
import helloworld_pb2
import helloworld_pb2_grpc


class AuthLoggingServerInterceptor(grpc.ServerInterceptor):
    def intercept_service(self, continuation, handler_call_details):
        handler = continuation(handler_call_details)
        if handler is None:
            return None

        metadata = dict(handler_call_details.invocation_metadata or [])
        logging.info("incoming metadata: %s", metadata)

        if handler.unary_unary:
            def auth_wrapper(request, context):
                auth = dict(context.invocation_metadata())
                token = auth.get("authorization")
                if token != "Bearer demo-token":
                    context.abort(grpc.StatusCode.UNAUTHENTICATED, "missing or invalid bearer token")
                traceparent = auth.get("traceparent")
                if traceparent:
                    logging.info("traceparent from incoming metadata: %s", traceparent)
                return handler.unary_unary(request, context)

            return grpc.unary_unary_rpc_method_handler(
                auth_wrapper,
                request_deserializer=handler.request_deserializer,
                response_serializer=handler.response_serializer,
            )

        return handler


class Greeter(helloworld_pb2_grpc.GreeterServicer):
    def SayHello(self, request, context):
        return helloworld_pb2.HelloReply(message=f"Hello, {request.name}!")


def serve(port: int = 50054):
    server = grpc.server(
        futures.ThreadPoolExecutor(max_workers=10),
        interceptors=(AuthLoggingServerInterceptor(),),
    )
    helloworld_pb2_grpc.add_GreeterServicer_to_server(Greeter(), server)
    server.add_insecure_port(f"[::]:{port}")
    server.start()
    logging.info("interceptor server listening on port %d", port)
    server.wait_for_termination()


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO, format="%(asctime)s %(levelname)s %(message)s")
    serve()
