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
         " [--port <port>] [--mtls] [--cert-dir <directory>]";
}

}  // namespace

class GreeterService final : public Greeter::Service {
 public:
  grpc::Status SayHello(grpc::ServerContext* context,
                        const HelloRequest* request,
                        HelloReply* response) override {
    response->set_message("Hello, " + request->name() + "!");
    return grpc::Status::OK;
  }
};

int main(int argc, char** argv) {
  int port = 50051;
  bool mtls = false;
  std::string cert_dir = "certs";

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--port" && i + 1 < argc) {
      port = std::stoi(argv[++i]);
    } else if (arg == "--cert-dir" && i + 1 < argc) {
      cert_dir = argv[++i];
    } else if (arg == "--mtls") {
      mtls = true;
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
    GreeterService service;
    grpc::ServerBuilder builder;

    grpc::SslServerCredentialsOptions ssl_opts;
    grpc::SslServerCredentialsOptions::PemKeyCertPair key_cert_pair;
    key_cert_pair.private_key = ReadFile(cert_dir + "/server.key");
    key_cert_pair.cert_chain = ReadFile(cert_dir + "/server.crt");
    ssl_opts.pem_key_cert_pairs.push_back(key_cert_pair);

    if (mtls) {
      ssl_opts.pem_root_certs = ReadFile(cert_dir + "/ca.crt");
      ssl_opts.force_client_auth = true;
    }

    std::string address = "0.0.0.0:" + std::to_string(port);
    builder.AddListeningPort(address, grpc::SslServerCredentials(ssl_opts));
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server = builder.BuildAndStart();
    if (!server) {
      std::cerr << "Failed to start server\n";
      return 1;
    }

    std::cout << "Secure gRPC server listening on " << address << "\n";
    server->Wait();
  } catch (const std::exception& ex) {
    std::cerr << "Error: " << ex.what() << '\n';
    return 1;
  }

  return 0;
}
