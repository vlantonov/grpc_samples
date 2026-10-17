# grpc_samples
Basic gRPC examples

## Available samples

### Python samples
Python examples focused on quick iteration and readability, covering core RPC patterns and production-oriented features.

* [Python samples index](./Python/README.md) - entry point for all Python examples.
* [Python basics](./Python/basic/README.md) - unary RPC hello-world style sample.
* [Python bidirectional streaming with backpressure](./Python/bidi_backpressure/README.md) - bidi stream behavior under slower consumers.
* [Python gRPC health and reflection](./Python/health_reflection/README.md) - health checks and reflection service setup.
* [Python gRPC middleware / interceptors](./Python/interceptors/README.md) - auth, logging, and trace-context propagation.
* [Python TLS / mTLS sample](./Python/tls/README.md) - secure channels with certificate-based auth.

### C++ samples
C++ examples focused on performance patterns, callback/reactor APIs, and deployment-ready features.

* [C++ samples index](./Cpp/README.md) - grouped overview of all C++ examples.
* [C++ bidirectional streaming with backpressure](./Cpp/BiDirectionalStreamingBackpressureExample/README.md) - streaming pressure and flow behavior demo.
* [C++ gRPC health and reflection](./Cpp/HealthReflectionExample/README.md) - health checks and reflection in C++ servers.
* [C++ gRPC middleware / interceptors](./Cpp/InterceptorExample/README.md) - client/server interceptor pipeline examples.
* [C++ TLS / mTLS sample](./Cpp/TlsExample/README.md) - secure transport and mutual authentication.

### Cross-language comparison
Shared-proto benchmark sample to compare Python and C++ unary RPC throughput and latency.

* [C++ vs Python performance comparison](./PerformanceComparison/README.md)

## Links
* [Define a gRPC liveness probe](https://kubernetes.io/docs/tasks/configure-pod-container/configure-liveness-readiness-startup-probes/#define-a-grpc-liveness-probe)
* [Ensuring gRPC Server Health Checks on Kubernetes: A Comprehensive Guide](https://medium.com/@blackhorseya/ensuring-grpc-server-health-checks-on-kubernetes-a-comprehensive-guide-86aac08ad5b0)
