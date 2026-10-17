#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <grpc++/grpc++.h>

#include "helloworld.grpc.pb.h"

using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;

int main(int argc, char** argv) {
  std::string host = "localhost";
  int port = 50071;
  int count = 1000;

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--host" && i + 1 < argc) {
      host = argv[++i];
    } else if (arg == "--port" && i + 1 < argc) {
      port = std::stoi(argv[++i]);
    } else if (arg == "--count" && i + 1 < argc) {
      count = std::stoi(argv[++i]);
    } else if (arg == "--help") {
      std::cout << "Usage: " << argv[0]
                << " [--host <host>] [--port <port>] [--count <count>]\n";
      return 0;
    } else {
      std::cerr << "Unknown argument: " << arg << "\n";
      return 1;
    }
  }

  auto channel = grpc::CreateChannel(host + ":" + std::to_string(port),
                                     grpc::InsecureChannelCredentials());
  std::unique_ptr<Greeter::Stub> stub = Greeter::NewStub(channel);

  std::vector<double> latencies;
  latencies.reserve(count);

  auto start = std::chrono::steady_clock::now();
  for (int i = 0; i < count; ++i) {
    grpc::ClientContext context;
    HelloRequest request;
    request.set_name("world");
    HelloReply reply;

    auto t0 = std::chrono::steady_clock::now();
    grpc::Status status = stub->SayHello(&context, request, &reply);
    auto elapsed_ms = std::chrono::duration<double, std::milli>(
                          std::chrono::steady_clock::now() - t0)
                          .count();
    latencies.push_back(elapsed_ms);

    if (!status.ok() || reply.message() != "Hello, world!") {
      std::cerr << "RPC failed: " << status.error_code() << ": "
                << status.error_message() << "\n";
      return 1;
    }
  }
  auto elapsed = std::chrono::duration<double>(
                     std::chrono::steady_clock::now() - start)
                     .count();

  double avg = 0.0;
  for (double v : latencies) {
    avg += v;
  }
  avg /= static_cast<double>(latencies.size());

  std::sort(latencies.begin(), latencies.end());
  double p95 = latencies.at(std::max<int>(0, (latencies.size() * 95) / 100 - 1));

  std::ostringstream out;
  out << "{\"language\":\"cpp\",\"count\":" << count << ",\"elapsed_sec\":"
      << elapsed << ",\"avg_latency_ms\":" << avg << ",\"p95_latency_ms\":"
      << p95 << ",\"rps\":" << (count / elapsed) << "}";
  std::cout << out.str() << "\n";
  return 0;
}
