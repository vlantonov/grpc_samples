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

class Reader : public ClientReadReactor<HelloReply> {
 public:
  Reader(Greeter::Stub* stub, HelloRequest* request, HelloReply* reply)
      : mReply(reply) {
    stub->async()->TestServerStream(&mContext, request, this);
    StartRead(mReply);
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

  void OnReadDone(bool ok) override {
    std::cout << "OnReadDone " << ok << " \n";
    if (ok) {
      std::cout << "Received reply " << mReply->message() << '\n';
      StartRead(mReply);
    }
  }

  Status Await() {
    std::unique_lock l{mMutex};
    mCv.wait(l, [this] { return mDone; });
    return std::move(mStatus);
  }

 private:
  ClientContext mContext;
  HelloReply* mReply;

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

  HelloRequest request;
  request.set_name("world");

  HelloReply reply;

  Reader reader(stub.get(), &request, &reply);
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
