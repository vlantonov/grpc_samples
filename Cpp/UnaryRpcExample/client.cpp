// https://grpc.io/docs/languages/cpp/callback/
// Unary RPC

#include <condition_variable>
#include <iostream>

#include "grpc++/grpc++.h"
#include "helloworld.grpc.pb.h"

using namespace helloworld;
using namespace grpc;

class Reader : public ClientUnaryReactor {
 public:
  Reader(Greeter::Stub* stub, HelloRequest* request, HelloReply* reply) {
    stub->async()->TestUnary(&mContext, request, reply, this);
    StartCall();
  }

  void OnDone(const grpc::Status& s) override {
    std::cout << "OnDone " << s.error_message() << " \n";
    std::unique_lock l{mMutex};
    mStatus = s;
    mDone = true;
    mCv.notify_one();
  }

  void OnReadInitialMetadataDone(bool ok) override {
    std::cout << "OnReadInitialMetadataDone " << ok << "\n";
  }

  Status Await() {
    std::unique_lock l{mMutex};
    mCv.wait(l, [this] { return mDone; });
    return std::move(mStatus);
  }

 private:
  ClientContext mContext;

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
    std::cout << "reply: " << reply.message() << '\n';
  } else {
    std::cout << "rpc failed." << '\n';
    std::cout << "rpc error: " << status.error_code() << ": "
              << status.error_message() << '\n';
  }

  std::cout << "Done\n";

  return 0;
}
