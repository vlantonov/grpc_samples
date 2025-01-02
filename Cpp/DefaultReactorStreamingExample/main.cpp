#include <grpc++/ext/proto_server_reflection_plugin.h>
#include <grpc++/grpc++.h>

#include <thread>

#include "helloworld.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;

class GreeterServiceImpl final : public Greeter::Service {
  Status SayHello(ServerContext* context, const HelloRequest* request,
                  HelloReply* reply) override {
    std::string prefix("Reply: Hi there ");
    reply->set_message(prefix + request->name());
    return Status::OK;
  }
};

constexpr auto kServerAddress = "127.0.0.1:50051";

std::unique_ptr<Server> server;

void RunServer() {
  GreeterServiceImpl service;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(kServerAddress, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  server = builder.BuildAndStart();
  std::cout << "Server listening on " << kServerAddress << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

class GreeterClient {
 public:
  GreeterClient(std::shared_ptr<Channel> channel)
      : mStub(Greeter::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  std::string SayHello(const std::string& user) {
    // Data we are sending to the server.
    HelloRequest request;
    request.set_name(user);

    // Container for the data we expect from the server.
    HelloReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = mStub->SayHello(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
      return reply.message();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

 private:
  std::unique_ptr<Greeter::Stub> mStub;
};

int main() {
  std::jthread serverThread(RunServer);

  GreeterClient greeter(
      grpc::CreateChannel(kServerAddress, grpc::InsecureChannelCredentials()));
  std::string user("world");
  std::string reply = greeter.SayHello(user);
  std::cout << "Greeter received: " << reply << std::endl;

  std::this_thread::sleep_for(std::chrono::seconds(5));
  std::cout << "Shutting down\n";
  if (server) {
    server->Shutdown();
  }

  std::cout << "Done.\n";
  return 0;
}