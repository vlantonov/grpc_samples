// https://grpc.io/docs/languages/cpp/callback/
// Server-side streaming RPC

#include <grpc++/grpc++.h>

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

class Streamer : public ServerWriteReactor<HelloReply> {
 public:
  Streamer(const std::vector<HelloReply>* responsesList)
      : mResponseList(responsesList), mNextFeature(mResponseList->begin()) {
    NextWrite();
  }

  void OnWriteDone(bool ok) override {
    std::cout << "Streamer OnWriteDone " << ok << "\n";
    if (!ok) {
      Finish(Status(grpc::StatusCode::UNKNOWN, "Unexpected HelloReply"));
    }
    NextWrite();
  }

  void OnDone() override {
    std::cout << "Streamer Completed\n";
    delete this;
  }

  void OnCancel() override { std::cout << "Streamer Cancelled\n"; }

 private:
  void NextWrite() {
    while (mNextFeature != mResponseList->end()) {
      const auto& f = *mNextFeature;
      mNextFeature++;
      if (true) {
        StartWrite(&f);
        return;
      }
    }
    // Didn't write anything, all is done.
    Finish(Status::OK);
  }

  const std::vector<HelloReply>* mResponseList;
  std::vector<HelloReply>::const_iterator mNextFeature;
};

class GreeterServiceImpl final : public Greeter::CallbackService {
  ServerWriteReactor<HelloReply>* TestServerStream(
      CallbackServerContext* context, const HelloRequest* request) override {
    if (!mResponseMap.contains(request->name())) {
      mResponseMap[request->name()] = mResponses;
      for (auto& response : mResponseMap[request->name()]) {
        response.set_message(request->name() + ":" + response.message());
      }
    }

    return new Streamer(&mResponseMap[request->name()]);
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