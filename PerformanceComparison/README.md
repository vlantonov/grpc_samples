# C++ vs Python gRPC performance comparison

This sample measures the same unary RPC in both Python and C++ using the same proto definition and reports:

- requests per second (RPS)
- average latency in milliseconds
- total elapsed time

## Shared proto

The project uses a single proto file in `proto/helloworld.proto`.

## Python benchmark

```bash
cd python
python3 -m grpc_tools.protoc -I../proto --python_out=. --grpc_python_out=. ../proto/helloworld.proto
python3 greeter_server.py
```

Then, in another terminal:

```bash
cd python
python3 greeter_client.py --count 1000
```

## C++ benchmark

```bash
cd cpp
conan install . --output-folder=build --build=missing
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=build/build/Release/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/perf_server --port 50071
```

Then, in another terminal:

```bash
cd cpp
./build/perf_client --host localhost --port 50071 --count 1000
```

## Cross-language comparison

```bash
cd PerformanceComparison
python3 compare.py --count 500
```

The script starts both backends, benchmarks each implementation, and prints a compact side-by-side summary.
