import argparse
import logging
from concurrent import futures

import grpc

import helloworld_pb2
import helloworld_pb2_grpc


class Greeter(helloworld_pb2_grpc.GreeterServicer):
    def SayHello(self, request, context):
        return helloworld_pb2.HelloReply(message=f"Hello, {request.name}!")


def load_server_credentials(cert_dir: str, mtls: bool):
    with open(f"{cert_dir}/server.key", "rb") as handle:
        server_key = handle.read()
    with open(f"{cert_dir}/server.crt", "rb") as handle:
        server_cert = handle.read()

    root_certificates = None
    if mtls:
        with open(f"{cert_dir}/ca.crt", "rb") as handle:
            root_certificates = handle.read()

    return grpc.ssl_server_credentials(
        private_key_certificate_chain_pairs=[(server_key, server_cert)],
        root_certificates=root_certificates,
        require_client_auth=mtls,
    )


def serve():
    parser = argparse.ArgumentParser(description="Run a secure gRPC server")
    parser.add_argument("--port", type=int, default=50051, help="Port to listen on")
    parser.add_argument("--cert-dir", default="certs", help="Directory containing cert files")
    parser.add_argument("--mtls", action="store_true", help="Require and verify client certificates")
    args = parser.parse_args()

    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    helloworld_pb2_grpc.add_GreeterServicer_to_server(Greeter(), server)

    credentials = load_server_credentials(args.cert_dir, args.mtls)
    server.add_secure_port(f"[::]:{args.port}", credentials)
    server.start()

    mode = "mTLS" if args.mtls else "TLS"
    logging.info("%s gRPC server listening on port %d", mode, args.port)
    server.wait_for_termination()


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO)
    serve()
