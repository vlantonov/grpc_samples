// https://grpc.io/docs/languages/cpp/callback/
// Client-side streaming RPC

#include <grpc++/grpc++.h>

#include <iostream>

#include "helloworld.grpc.pb.h"

using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;

using grpc::CallbackServerContext;
using grpc::Server;
using grpc::ServerBidiReactor;
using grpc::ServerBuilder;
using grpc::Status;

class Streamer : public ServerBidiReactor<HelloRequest, HelloReply> {
 public:
  Streamer(std::mutex& aMutex, std::vector<HelloRequest>* receivedRequests)
      : mMutex(aMutex), mReceivedRequests(receivedRequests) {
    StartRead(&mRequest);
  }

  void OnReadDone(bool ok) override {
    std::cout << "OnReadDone " << ok << " \n";
    if (ok) {
      // Unlike the other example in this directory that's not using
      // the reactor pattern, we can't grab a local lock to secure the
      // access to the notes vector, because the reactor will most likely
      // make us jump threads, so we'll have to use a different locking
      // strategy. We'll grab the lock locally to build a copy of the
      // list of nodes we're going to send, then we'll grab the lock
      // again to append the received note to the existing vector.
      std::cout << "Received request " << mRequest.name() << '\n';
      {
        std::lock_guard lock{mMutex};
        const auto itFound =
            std::find_if(mReceivedRequests->begin(), mReceivedRequests->end(),
                         [this](const auto& aRequest) {
                           return aRequest.name() == mRequest.name();
                         });
        if (itFound == mReceivedRequests->end()) {
          HelloReply response;
          const static std::string prefix("Hi there ");
          response.set_message(prefix + mRequest.name());
          std::cout << "Prepared reply " << response.message() << '\n';
          mToSendReplies.emplace_back(std::move(response));
        }
      }
      NextWrite();
    } else {
      Finish(Status::OK);
    }
  }

  void OnWriteDone(bool ok) override {
    std::cout << "OnWriteDone " << ok << " \n";
    NextWrite();
  }

  void OnDone() override {
    std::cout << "Streamer Completed\n";
    delete this;
  }

  void OnCancel() override { std::cout << "Streamer Cancelled\n"; }

 private:
  void NextWrite() {
    if (mRepliesIterator < mToSendReplies.size()) {
      StartWrite(&(mToSendReplies[mRepliesIterator]));
      mRepliesIterator++;
    } else {
      {
        std::lock_guard lock{mMutex};
        mReceivedRequests->push_back(mRequest);
      }
      StartRead(&mRequest);
    }
  }

  std::mutex& mMutex;
  HelloRequest mRequest;
  std::vector<HelloRequest>* mReceivedRequests;
  std::vector<HelloReply> mToSendReplies;
  size_t mRepliesIterator{0};
};

class GreeterServiceImpl final : public Greeter::CallbackService {
  ServerBidiReactor<HelloRequest, HelloReply>* TestBiDirectionStream(
      CallbackServerContext* context) override {
    return new Streamer(mMutex, &mReceivedRequests);
  }

  std::mutex mMutex;
  std::vector<HelloRequest> mReceivedRequests;
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