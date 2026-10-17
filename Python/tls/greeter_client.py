import argparse
import logging

import grpc

import helloworld_pb2
import helloworld_pb2_grpc


def load_channel_credentials(cert_dir: str, mtls: bool):
    with open(f"{cert_dir}/ca.crt", "rb") as handle:
        root_certificates = handle.read()

    if not mtls:
        return grpc.ssl_channel_credentials(root_certificates=root_certificates)

    with open(f"{cert_dir}/client.key", "rb") as handle:
        private_key = handle.read()
    with open(f"{cert_dir}/client.crt", "rb") as handle:
        certificate_chain = handle.read()

    return grpc.ssl_channel_credentials(
        root_certificates=root_certificates,
        private_key=private_key,
        certificate_chain=certificate_chain,
    )


def run():
    parser = argparse.ArgumentParser(description="Call a secure gRPC server")
    parser.add_argument("--host", default="localhost", help="Server hostname")
    parser.add_argument("--port", type=int, default=50051, help="Server port")
    parser.add_argument("--cert-dir", default="certs", help="Directory containing cert files")
    parser.add_argument("--mtls", action="store_true", help="Present the client certificate")
    parser.add_argument("--name", default="you", help="Name to send in the request")
    args = parser.parse_args()

    credentials = load_channel_credentials(args.cert_dir, args.mtls)
    with grpc.secure_channel(f"{args.host}:{args.port}", credentials) as channel:
        stub = helloworld_pb2_grpc.GreeterStub(channel)
        response = stub.SayHello(helloworld_pb2.HelloRequest(name=args.name))

    print(f"Greeter client received: {response.message}")


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO)
    run()
