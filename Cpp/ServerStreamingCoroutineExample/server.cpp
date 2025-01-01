// https://grpc.io/docs/languages/cpp/callback/
// Server-side streaming RPC

#include <grpc++/grpc++.h>

#include <coroutine>
#include <iostream>

#include "helloworld.grpc.pb.h"

using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;

using grpc::CallbackServerContext;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerWriteReactor;
using grpc::Status;

class ServerWriteReactorPromiseType;

class Streamer : public ServerWriteReactor<HelloReply> {
 public:
  explicit Streamer(ServerWriteReactorPromiseType& p)
      : m_coroutine_handle{
            std::coroutine_handle<ServerWriteReactorPromiseType>::from_promise(
                p)} {}

  // Loop 3/4
  void OnWriteDone(bool ok) override {
    std::cout << "Streamer OnWriteDone " << ok << "\n";
    if (ok) {
      m_coroutine_handle.resume();
    } else {
      // RPC cancelled – no point in continuing with coroutine
      m_coroutine_handle.destroy();
    }
  }

  // Clean up the Reactor
  void OnDone() override {
    std::cout << "Streamer Completed\n";
    delete this;
  }

  void OnCancel() override { std::cout << "Streamer Cancelled\n"; }

 public:
  std::coroutine_handle<ServerWriteReactorPromiseType> m_coroutine_handle{};
};

struct ServerWriteReactorPromiseType {
  struct SendResponseAwaiter {
    // Always suspend
    constexpr bool await_ready() noexcept { return false; }

    // Returns void
    // -> Always return control to coroutine caller/resumer
    void await_suspend(
        std::coroutine_handle<ServerWriteReactorPromiseType> coroutine_handle) {
      auto& promise = coroutine_handle.promise();
      promise.mReactor->m_coroutine_handle = coroutine_handle;
      promise.mReactor->StartWrite(&mResponse);
    }

    constexpr void await_resume() noexcept {
      // Do nothing
    }

    const HelloReply& mResponse;
  };

  Streamer* mReactor;

  void return_value(Status status) noexcept { finish(status); }

  void finish(Status status) {
    assert(mReactor);
    mReactor->Finish(status);
    mReactor = nullptr;
  }

  // Destructor: Finish if not already finished
  ~ServerWriteReactorPromiseType() {
    if (mReactor) {
      finish(Status::CANCELLED);
    }
  }

  ServerWriteReactor<HelloReply>* get_return_object() {
    mReactor = new Streamer{*this};
    return mReactor;
  }

  auto initial_suspend() { return std::suspend_never{}; }

  auto final_suspend() noexcept { return std::suspend_never{}; }

  void unhandled_exception() {
    finish(Status{grpc::StatusCode::UNKNOWN, "Unhandled Exception"});
  }

  // Required for co_await response;
  // auto await_transform(const HelloReply& response) noexcept {
  //   return SendResponseAwaiter{response};
  // }
};

ServerWriteReactorPromiseType::SendResponseAwaiter send(
    const HelloReply& response) {
  return {response};
}

template <typename... Args>
struct std::coroutine_traits<ServerWriteReactor<HelloReply>*, Args...> {
  using promise_type = ServerWriteReactorPromiseType;
};

class GreeterServiceImpl final : public Greeter::CallbackService {
  ServerWriteReactor<HelloReply>* TestServerStream(
      CallbackServerContext* context, const HelloRequest* request) override {
    std::cout << "Received request: " << request->name() << "\n";

    if (!mResponseMap.contains(request->name())) {
      std::cout << "Initialize requests\n";
      mResponseMap[request->name()] = mResponses;
      for (auto& response : mResponseMap[request->name()]) {
        response.set_message(request->name() + ":" + response.message());
      }
    }

    for (auto& response : mResponseMap[request->name()]) {
      co_await send(response);

      // Requires await_transform
      // co_await response;
    }

    co_return Status::OK;
  }

  std::vector<HelloReply> mResponses = []() {
    std::vector<HelloReply> data;
    for (auto elem : {"1a", "2b", "3c", "4d"}) {
      HelloReply reply;
      reply.set_message(elem);
      data.push_back(reply);
    }
    return data;
  }();

  std::map<std::string, std::vector<HelloReply>> mResponseMap;
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
