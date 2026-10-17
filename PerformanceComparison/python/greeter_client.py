import argparse
import json
import time

import grpc

import helloworld_pb2
import helloworld_pb2_grpc


def benchmark(host: str, port: int, count: int):
    channel = grpc.insecure_channel(f"{host}:{port}")
    stub = helloworld_pb2_grpc.GreeterStub(channel)

    latencies = []
    start = time.perf_counter()
    for _ in range(count):
        t0 = time.perf_counter()
        response = stub.SayHello(helloworld_pb2.HelloRequest(name="world"))
        elapsed_ms = (time.perf_counter() - t0) * 1000.0
        latencies.append(elapsed_ms)
        if response.message != "Hello, world!":
            raise RuntimeError(f"unexpected response: {response.message!r}")
    elapsed_sec = time.perf_counter() - start

    result = {
        "language": "python",
        "count": count,
        "elapsed_sec": elapsed_sec,
        "avg_latency_ms": sum(latencies) / len(latencies),
        "p95_latency_ms": sorted(latencies)[max(0, int(len(latencies) * 0.95) - 1)],
        "rps": count / elapsed_sec,
    }
    print(json.dumps(result))


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Benchmark the Python gRPC unary call")
    parser.add_argument("--host", default="localhost")
    parser.add_argument("--port", type=int, default=50070)
    parser.add_argument("--count", type=int, default=1000)
    args = parser.parse_args()
    benchmark(args.host, args.port, args.count)
