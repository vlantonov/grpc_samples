#include <iostream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>
#include <grpcpp/support/client_interceptor.h>

#include "helloworld.grpc.pb.h"

using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;

class AuthTraceClientInterceptor final : public grpc::experimental::Interceptor {
 public:
  void Intercept(grpc::experimental::InterceptorBatchMethods* methods) override {
    if (methods->QueryInterceptionHookPoint(
            grpc::experimental::InterceptionHookPoints::PRE_SEND_INITIAL_METADATA)) {
      auto* metadata = methods->GetSendInitialMetadata();
      if (metadata) {
        metadata->emplace("authorization", "Bearer demo-token");
        metadata->emplace("traceparent",
                          "00-4bf92f3577b34da6a3ce929d0e0e4736-00f067aa0ba902b7-01");
        std::cout << "client sending metadata:\n";
        for (const auto& [key, value] : *metadata) {
          std::cout << "  " << key << ": " << value << "\n";
        }
      }
    }
    methods->Proceed();
  }
};

class AuthTraceClientInterceptorFactory final
    : public grpc::experimental::ClientInterceptorFactoryInterface {
 public:
  grpc::experimental::Interceptor* CreateClientInterceptor(
      grpc::experimental::ClientRpcInfo* info) override {
    return new AuthTraceClientInterceptor();
  }
};

int main(int argc, char** argv) {
  std::string host = "localhost";
  int port = 50054;
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

  grpc::ChannelArguments args;
  std::vector<std::unique_ptr<grpc::experimental::ClientInterceptorFactoryInterface>>
      factories;
  factories.push_back(std::make_unique<AuthTraceClientInterceptorFactory>());

  auto channel = grpc::experimental::CreateCustomChannelWithInterceptors(
      host + ":" + std::to_string(port), grpc::InsecureChannelCredentials(),
      args, std::move(factories));
  std::unique_ptr<Greeter::Stub> stub = Greeter::NewStub(channel);

  grpc::ClientContext context;
  HelloRequest request;
  request.set_name("world");
  HelloReply reply;

  grpc::Status status = stub->SayHello(&context, request, &reply);
  if (!status.ok()) {
    std::cerr << "RPC failed: " << status.error_code() << ": "
              << status.error_message() << "\n";
    return 1;
  }

  std::cout << "Greeter client received: " << reply.message() << "\n";
  return 0;
}
