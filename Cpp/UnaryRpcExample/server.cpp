// https://grpc.io/docs/languages/cpp/callback/
// Unary RPC

#include <grpc++/grpc++.h>

#include <iostream>

#include "helloworld.grpc.pb.h"

using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;

using grpc::CallbackServerContext;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerUnaryReactor;
using grpc::Status;

class GreeterServiceImpl final : public Greeter::CallbackService {
  ServerUnaryReactor* TestUnary(CallbackServerContext* context,
                                const HelloRequest* request,
                                HelloReply* response) override {
    std::string prefix("Reply: Hi there ");
    response->set_message(prefix + request->name());

    auto* reactor = context->DefaultReactor();
    reactor->Finish(Status::OK);
    return reactor;
  }
};

int main() {
  GreeterServiceImpl service;
  ServerBuilder builder;

  builder.AddListeningPort("127.0.0.1:50051",
                           grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  server->Wait();

  return 0;
}