#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include <grpc++/grpc++.h>

#include "helloworld.grpc.pb.h"

using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;

namespace {

std::string ReadFile(const std::string& path) {
  std::ifstream stream(path, std::ios::binary);
  if (!stream) {
    throw std::runtime_error("failed to open file: " + path);
  }

  return std::string((std::istreambuf_iterator<char>(stream)),
                     std::istreambuf_iterator<char>());
}

std::string BuildUsage(const char* argv0) {
  return std::string("Usage: ") + argv0 +
         " [--host <host>] [--port <port>] [--mtls] [--cert-dir <directory>]"
         " [--name <name>]";
}

}  // namespace

int main(int argc, char** argv) {
  std::string host = "localhost";
  int port = 50051;
  bool mtls = false;
  std::string cert_dir = "certs";
  std::string name = "you";

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--host" && i + 1 < argc) {
      host = argv[++i];
    } else if (arg == "--port" && i + 1 < argc) {
      port = std::stoi(argv[++i]);
    } else if (arg == "--cert-dir" && i + 1 < argc) {
      cert_dir = argv[++i];
    } else if (arg == "--mtls") {
      mtls = true;
    } else if (arg == "--name" && i + 1 < argc) {
      name = argv[++i];
    } else if (arg == "--help") {
      std::cout << BuildUsage(argv[0]) << '\n';
      return 0;
    } else {
      std::cerr << "Unknown argument: " << arg << '\n';
      std::cerr << BuildUsage(argv[0]) << '\n';
      return 1;
    }
  }

  try {
    grpc::SslCredentialsOptions ssl_opts;
    ssl_opts.pem_root_certs = ReadFile(cert_dir + "/ca.crt");

    if (mtls) {
      ssl_opts.pem_private_key = ReadFile(cert_dir + "/client.key");
      ssl_opts.pem_cert_chain = ReadFile(cert_dir + "/client.crt");
    }

    auto channel = grpc::CreateChannel(host + ":" + std::to_string(port),
                                       grpc::SslCredentials(ssl_opts));
    std::unique_ptr<Greeter::Stub> stub = Greeter::NewStub(channel);

    grpc::ClientContext context;
    HelloRequest request;
    request.set_name(name);

    HelloReply reply;
    grpc::Status status = stub->SayHello(&context, request, &reply);

    if (!status.ok()) {
      std::cerr << "RPC failed: " << status.error_code() << ": "
                << status.error_message() << '\n';
      return 1;
    }

    std::cout << "Greeter client received: " << reply.message() << '\n';
  } catch (const std::exception& ex) {
    std::cerr << "Error: " << ex.what() << '\n';
    return 1;
  }

  return 0;
}
