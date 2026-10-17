#include <iostream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>

#include "health.grpc.pb.h"
#include "helloworld.grpc.pb.h"

int main(int argc, char** argv) {
  std::string host = "localhost";
  int port = 50053;

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--host" && i + 1 < argc) {
      host = argv[++i];
    } else if (arg == "--port" && i + 1 < argc) {
      port = std::stoi(argv[++i]);
    } else if (arg == "--help") {
      std::cout << "Usage: " << argv[0] << " [--host <host>] [--port <port>]\n";
      return 0;
    } else {
      std::cerr << "Unknown argument: " << arg << "\n";
      return 1;
    }
  }

  auto channel = grpc::CreateChannel(host + ":" + std::to_string(port),
                                     grpc::InsecureChannelCredentials());

  auto health_stub = grpc::health::v1::Health::NewStub(channel);
  grpc::health::v1::HealthCheckRequest health_request;
  health_request.set_service("helloworld.Greeter");
  grpc::health::v1::HealthCheckResponse health_response;
  grpc::ClientContext health_context;
  grpc::Status health_status =
      health_stub->Check(&health_context, health_request, &health_response);

  if (!health_status.ok()) {
    std::cerr << "Health check failed: " << health_status.error_code() << ": "
              << health_status.error_message() << "\n";
    return 1;
  }

  std::cout << "Health status: " << health_response.status() << "\n";

  auto greeter_stub = helloworld::Greeter::NewStub(channel);
  grpc::ClientContext context;
  helloworld::HelloRequest request;
  request.set_name("world");
  helloworld::HelloReply reply;

  grpc::Status status = greeter_stub->SayHello(&context, request, &reply);
  if (!status.ok()) {
    std::cerr << "RPC failed: " << status.error_code() << ": "
              << status.error_message() << "\n";
    return 1;
  }

  std::cout << "Greeter client received: " << reply.message() << "\n";
  return 0;
}
