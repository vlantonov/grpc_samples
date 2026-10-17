#include <iostream>
#include <string>

#include <grpc++/grpc++.h>
#include <grpcpp/support/interceptor.h>
#include <grpcpp/support/server_interceptor.h>

#include "helloworld.grpc.pb.h"

using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;

class LoggingServerInterceptor final : public grpc::experimental::Interceptor {
 public:
  void Intercept(grpc::experimental::InterceptorBatchMethods* methods) override {
    if (methods->QueryInterceptionHookPoint(
            grpc::experimental::InterceptionHookPoints::PRE_RECV_INITIAL_METADATA)) {
      auto* metadata = methods->GetRecvInitialMetadata();
      if (metadata) {
        std::cout << "server received metadata:\n";
        for (const auto& [key, value] : *metadata) {
          std::cout << "  " << std::string(key.data(), key.size()) << ": "
                    << std::string(value.data(), value.size()) << "\n";
        }
      }
    }
    methods->Proceed();
  }
};

class LoggingServerInterceptorFactory final
    : public grpc::experimental::ServerInterceptorFactoryInterface {
 public:
  grpc::experimental::Interceptor* CreateServerInterceptor(
      grpc::experimental::ServerRpcInfo* info) override {
    return new LoggingServerInterceptor();
  }
};

class GreeterService final : public Greeter::Service {
 public:
  grpc::Status SayHello(grpc::ServerContext* context,
                        const HelloRequest* request,
                        HelloReply* response) override {
    bool has_auth = false;
    bool has_traceparent = false;
    for (const auto& [key, value] : context->client_metadata()) {
      const std::string k(key.data(), key.size());
      const std::string v(value.data(), value.size());
      if (k == "authorization" && v == "Bearer demo-token") {
        has_auth = true;
      }
      if (k == "traceparent") {
        has_traceparent = true;
        std::cout << "server traceparent: " << v << "\n";
      }
    }

    if (!has_auth) {
      return grpc::Status(grpc::StatusCode::UNAUTHENTICATED,
                          "missing or invalid bearer token");
    }

    if (!has_traceparent) {
      std::cout << "server traceparent: <missing>\n";
    }

    response->set_message("Hello, " + request->name() + "!");
    return grpc::Status::OK;
  }
};

int main(int argc, char** argv) {
  int port = 50054;
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
  std::vector<std::unique_ptr<grpc::experimental::ServerInterceptorFactoryInterface>>
      factories;
  factories.push_back(std::make_unique<LoggingServerInterceptorFactory>());
  builder.experimental().SetInterceptorCreators(std::move(factories));

  builder.AddListeningPort("0.0.0.0:" + std::to_string(port),
                          grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<grpc::Server> server = builder.BuildAndStart();
  if (!server) {
    std::cerr << "failed to start server\n";
    return 1;
  }

  std::cout << "interceptor server listening on :" << port << "\n";
  server->Wait();
  return 0;
}
