import argparse
import json
import socket
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent
PYTHON_DIR = ROOT / "python"
CPP_DIR = ROOT / "cpp"


def wait_for_port(host: str, port: int, timeout: float = 20.0):
    deadline = time.time() + timeout
    while time.time() < deadline:
        try:
            with socket.create_connection((host, port), timeout=0.5):
                return
        except OSError:
            time.sleep(0.1)
    raise TimeoutError(f"Timed out waiting for {host}:{port}")


def run_python_benchmark(count: int):
    subprocess.run(
        [sys.executable, "-m", "grpc_tools.protoc", "-I../proto", "--python_out=.", "--grpc_python_out=.", "../proto/helloworld.proto"],
        cwd=PYTHON_DIR,
        check=True,
        capture_output=True,
        text=True,
    )

    server = subprocess.Popen([sys.executable, "greeter_server.py"], cwd=PYTHON_DIR)
    try:
        wait_for_port("localhost", 50070)
        proc = subprocess.run(
            [sys.executable, "greeter_client.py", "--count", str(count)],
            cwd=PYTHON_DIR,
            capture_output=True,
            text=True,
            check=True,
        )
        return json.loads(proc.stdout.strip())
    finally:
        server.terminate()
        server.wait(timeout=10)


def run_cpp_benchmark(count: int):
    binary = CPP_DIR / "build" / "perf_server"
    client_binary = CPP_DIR / "build" / "perf_client"
    if not binary.exists() or not client_binary.exists():
        raise FileNotFoundError(
            f"C++ sample is not built. Run: cd {CPP_DIR} && conan install . --output-folder=build --build=missing && cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=build/build/Release/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release && cmake --build build"
        )

    server = subprocess.Popen([str(binary), "--port", "50071"], cwd=CPP_DIR)
    try:
        wait_for_port("localhost", 50071)
        proc = subprocess.run(
            [str(client_binary), "--host", "localhost", "--port", "50071", "--count", str(count)],
            cwd=CPP_DIR,
            capture_output=True,
            text=True,
            check=True,
        )
        return json.loads(proc.stdout.strip())
    finally:
        server.terminate()
        server.wait(timeout=10)


def main():
    parser = argparse.ArgumentParser(description="Compare Python and C++ gRPC performance on the same unary proto")
    parser.add_argument("--count", type=int, default=200)
    args = parser.parse_args()

    py_stats = run_python_benchmark(args.count)
    cpp_stats = run_cpp_benchmark(args.count)

    print("Language  Count  Avg latency (ms)  P95 latency (ms)  RPS")
    print(f"Python    {py_stats['count']:<5}  {py_stats['avg_latency_ms']:<16.3f}  {py_stats['p95_latency_ms']:<15.3f}  {py_stats['rps']:.2f}")
    print(f"C++       {cpp_stats['count']:<5}  {cpp_stats['avg_latency_ms']:<16.3f}  {cpp_stats['p95_latency_ms']:<15.3f}  {cpp_stats['rps']:.2f}")


if __name__ == "__main__":
    main()
