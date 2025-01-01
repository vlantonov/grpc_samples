// https://grpc.io/docs/languages/cpp/callback/
// Unary RPC

#include <grpc++/grpc++.h>

#include <iostream>
#include <coroutine>

#include "helloworld.grpc.pb.h"

using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;

using grpc::CallbackServerContext;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerUnaryReactor;
using grpc::Status;

struct ServerUnaryReactorPromiseType{
  struct UnaryReactor : public ServerUnaryReactor {
    void OnDone() override {
      // Clean ourselves up
      // This is a gRPC idiom
      std::cout << "RPC Completed\n";
      delete this;
    }

    void OnCancel() override { std::cout << "RPC Cancelled\n"; }
  };

  UnaryReactor* mReactor = nullptr;

  void return_value( HelloReply* response) {
    finish(Status::OK);
  }

  void finish(Status status) {
    assert(mReactor);
    mReactor->Finish(status);
    mReactor = nullptr;
  }

  // Destructor: Finish if not already finished
  ~ServerUnaryReactorPromiseType() {
    if (mReactor) {
      finish(Status::CANCELLED);
    }
  }

  ServerUnaryReactor* get_return_object() {
    mReactor = new UnaryReactor{};
    return mReactor;
  }

  constexpr std::suspend_never initial_suspend() {
    return {};
  }

  void unhandled_exception() {
    finish(Status{grpc::StatusCode::UNKNOWN, "Unhandled Exception"});
  }

  constexpr std::suspend_never final_suspend() noexcept {
    return {};
  }
};

template <typename... Args>
struct std::coroutine_traits<ServerUnaryReactor*, Args...> {
  using promise_type = ServerUnaryReactorPromiseType;
};


class GreeterServiceImpl final : public Greeter::CallbackService {
  ServerUnaryReactor* TestUnary(CallbackServerContext* context,
                                const HelloRequest* request,
                                HelloReply* response) override {
    std::string prefix("Reply: Hi there ");
    response->set_message(prefix + request->name());
    co_return response;
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