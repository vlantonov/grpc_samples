#include <chrono>
#include <iostream>
#include <thread>

#include <grpc++/grpc++.h>

#include "chat.grpc.pb.h"

using chat::ChatReply;
using chat::ChatRequest;
using chat::ChatService;

class ChatServiceImpl final : public ChatService::Service {
 public:
  grpc::Status Chat(grpc::ServerContext* context,
                   grpc::ServerReaderWriter<ChatReply, ChatRequest>* stream)
      override {
    ChatRequest request;
    while (stream->Read(&request)) {
      std::cout << "server received: " << request.name() << " (seq="
                << request.sequence() << ")\n";

      std::this_thread::sleep_for(std::chrono::milliseconds(750));

      ChatReply reply;
      reply.set_message("processed: " + request.name());
      reply.set_sequence(request.sequence());
      if (!stream->Write(reply)) {
        std::cout << "client stopped reading early\n";
        break;
      }
    }

    return grpc::Status::OK;
  }
};

int main(int argc, char** argv) {
  int port = 50052;
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--port" && i + 1 < argc) {
      port = std::stoi(argv[++i]);
    } else if (arg == "--help") {
      std::cout << "Usage: " << argv[0] << " [--port <port>]\n";
      return 0;
    } else {
      std::cerr << "Unknown argument: " << arg << "\n";
      return 1;
    }
  }

  ChatServiceImpl service;
  grpc::ServerBuilder builder;
  builder.AddListeningPort("0.0.0.0:" + std::to_string(port),
                          grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<grpc::Server> server = builder.BuildAndStart();
  if (!server) {
    std::cerr << "failed to start server\n";
    return 1;
  }

  std::cout << "Bidirectional backpressure server listening on :" << port
            << "\n";
  server->Wait();
  return 0;
}
