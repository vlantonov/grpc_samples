// https://grpc.io/docs/languages/cpp/callback/
// Client-side streaming RPC

#include <grpc++/grpc++.h>

#include <condition_variable>
#include <iostream>

#include "helloworld.grpc.pb.h"

using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;

using grpc::ClientBidiReactor;
using grpc::ClientContext;
using grpc::Status;

class Streamer : public ClientBidiReactor<HelloRequest, HelloReply> {
 public:
  Streamer(Greeter::Stub* stub, const std::vector<HelloRequest>* requests)
      : mRequests(requests), mRequestsRemaining(requests->size()) {
    stub->async()->TestBiDirectionStream(&mContext, this);
    NextWrite();
    StartRead(&mReply);
    StartCall();
  }

  void OnDone(const Status& s) override {
    std::cout << "OnDone " << s.error_message() << " \n";
    std::unique_lock l{mMutex};
    mStatus = s;
    mDone = true;
    mCv.notify_one();
  }

  void OnReadInitialMetadataDone(bool ok) override {
    std::cout << "OnReadInitialMetadataDone " << ok << "\n";
  }

  void OnWriteDone(bool ok) override {
    std::cout << "OnWriteDone " << ok << " \n";
    if (true) {
      NextWrite();
    }
  }

  void OnReadDone(bool ok) override {
    std::cout << "OnReadDone " << ok << " \n";
    if (ok) {
      std::cout << "OnReadDone Received response " << mReply.message() << '\n';
      StartRead(&mReply);
    }
  }

  Status Await() {
    std::unique_lock l{mMutex};
    mCv.wait(l, [this] { return mDone; });
    return std::move(mStatus);
  }

 private:
  void NextWrite() {
    if (mRequestsRemaining != 0) {
      StartWrite(&(*mRequests)[mRequestsRemaining - 1]);
      mRequestsRemaining--;
    } else {
      std::cout << "StartWritesDone " << " \n";
      StartWritesDone();
    }
  }

  ClientContext mContext;
  HelloReply mReply;
  const std::vector<HelloRequest>* mRequests;
  int mRequestsRemaining;

  std::mutex mMutex;
  std::condition_variable mCv;
  Status mStatus;
  bool mDone = false;
};

int main() {
  auto channel =
      CreateChannel("127.0.0.1:50051", grpc::InsecureChannelCredentials());
  auto stub = Greeter::NewStub(channel);
  ClientContext context;

  HelloReply reply;

  const std::vector<HelloRequest> requests = []() {
    std::vector<HelloRequest> data;
    for (auto elem : {"world 1a", "world 2b", "world 3c", "world 4d"}) {
      HelloRequest request;
      request.set_name(elem);
      data.push_back(request);
    }
    return data;
  }();

  Streamer reader(stub.get(), &requests);
  Status status = reader.Await();
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
