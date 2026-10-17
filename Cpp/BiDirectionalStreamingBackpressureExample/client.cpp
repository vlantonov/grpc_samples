#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include <grpc++/grpc++.h>

#include "chat.grpc.pb.h"

using chat::ChatReply;
using chat::ChatRequest;
using chat::ChatService;

int main(int argc, char** argv) {
  std::string host = "localhost";
  int port = 50052;
  int count = 10;
  int send_delay_ms = 200;

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--host" && i + 1 < argc) {
      host = argv[++i];
    } else if (arg == "--port" && i + 1 < argc) {
      port = std::stoi(argv[++i]);
    } else if (arg == "--count" && i + 1 < argc) {
      count = std::stoi(argv[++i]);
    } else if (arg == "--send-delay-ms" && i + 1 < argc) {
      send_delay_ms = std::stoi(argv[++i]);
    } else if (arg == "--help") {
      std::cout << "Usage: " << argv[0]
                << " [--host <host>] [--port <port>] [--count <count>]"
                   " [--send-delay-ms <ms>]\n";
      return 0;
    } else {
      std::cerr << "Unknown argument: " << arg << "\n";
      return 1;
    }
  }

  auto channel = grpc::CreateChannel(host + ":" + std::to_string(port),
                                     grpc::InsecureChannelCredentials());
  std::unique_ptr<ChatService::Stub> stub = ChatService::NewStub(channel);

  grpc::ClientContext context;
  std::shared_ptr<grpc::ClientReaderWriter<ChatRequest, ChatReply>> stream(
      stub->Chat(&context));

  std::cout << "sending requests to the bidirectional stream\n";

  for (int i = 1; i <= count; ++i) {
    ChatRequest request;
    request.set_name("client-" + std::to_string(i));
    request.set_sequence(i);
    if (!stream->Write(request)) {
      std::cout << "write failed before sending all messages\n";
      break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(send_delay_ms));
  }

  stream->WritesDone();

  ChatReply reply;
  while (stream->Read(&reply)) {
    std::cout << "received reply " << reply.sequence() << " -> "
              << reply.message() << "\n";
  }

  grpc::Status status = stream->Finish();
  if (!status.ok()) {
    std::cerr << "stream failed: " << status.error_code() << ": "
              << status.error_message() << "\n";
    return 1;
  }

  std::cout << "stream completed successfully\n";
  return 0;
}
