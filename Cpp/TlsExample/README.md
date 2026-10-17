# C++ TLS / mTLS gRPC sample

This sample demonstrates secure gRPC communication in C++ using TLS and mutual TLS (mTLS).

## Generate certificates

```bash
./generate_certs.sh
```

It creates a local CA and signed certificates under `certs/`:

- `ca.crt`
- `server.crt`
- `server.key`
- `client.crt`
- `client.key`

## Build

```bash
conan install . --output-folder=build --build=missing
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Run the TLS server

```bash
./tls_server --cert-dir ../certs
```

## Run the mTLS server

```bash
./tls_server --cert-dir ../certs --mtls
```

## Call the server over TLS

```bash
./tls_client --cert-dir ../certs
```

## Call the server over mTLS

```bash
./tls_client --cert-dir ../certs --mtls
```

The examples use `localhost` for the hostname, and the generated server certificate includes SAN entries for `localhost` and `127.0.0.1`.
