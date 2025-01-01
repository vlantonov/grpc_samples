// https://grpc.io/docs/languages/cpp/callback/
// Server-side streaming RPC

#include <grpc++/grpc++.h>

#include <condition_variable>
#include <iostream>

#include "helloworld.grpc.pb.h"

using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;

using grpc::ClientContext;
using grpc::ClientReadReactor;
using grpc::Status;

int main() {
  auto channel =
      CreateChannel("127.0.0.1:50051", grpc::InsecureChannelCredentials());
  auto stub = Greeter::NewStub(channel);
  ClientContext context;

  const auto request = []() {
    HelloRequest request;
    request.set_name("world");
    return request;
  }();

  HelloReply reply;

  std::unique_ptr<grpc::ClientReader<HelloReply> > reader(
      stub->TestServerStream(&context, request));
  while (reader->Read(&reply)) {
    std::cout << "Received reply " << reply.message() << '\n';
  }

  Status status = reader->Finish();
  if (status.ok()) {
    std::cout << "rpc succeeded." << '\n';
  } else {
    std::cout << "rpc failed." << '\n';
    std::cout << "rpc error: " << status.error_code() << ": "
              << status.error_message() << std::endl;
  }

  std::cout << "Done\n";

  return 0;
}
