#include <iostream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>

#include "helloworld.grpc.pb.h"

using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;

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
  int port = 50071;
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--port" && i + 1 < argc) {
      port = std::stoi(argv[++i]);
    } else if (arg == "--help") {
      std::cout << "Usage: " << argv[0] << " [--port <port>]\n";
      return 0;
    } else {
      std::cerr << "Unknown argument: " << arg << "\n";
      return 1;
    }
  }

  GreeterService service;
  grpc::ServerBuilder builder;
  builder.AddListeningPort("0.0.0.0:" + std::to_string(port),
                          grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<grpc::Server> server = builder.BuildAndStart();
  if (!server) {
    std::cerr << "failed to start server\n";
    return 1;
  }

  std::cout << "C++ server listening on :" << port << "\n";
  server->Wait();
  return 0;
}
