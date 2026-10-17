# TLS / mTLS gRPC sample

This sample demonstrates secure gRPC communication in Python using TLS, and mutual TLS (mTLS) with client certificate validation.

## Generate certificates

```bash
./generate_certs.sh
```

The script creates a local CA and signed certificates under `certs/`:

- `ca.crt`
- `server.crt`
- `server.key`
- `client.crt`
- `client.key`

## Run the TLS server

```bash
python3 greeter_server.py
```

## Run the mTLS server

```bash
python3 greeter_server.py --mtls
```

## Call the server over TLS

```bash
python3 greeter_client.py
```

## Call the server over mTLS

```bash
python3 greeter_client.py --mtls
```

The examples use `localhost` as the target address, so the server certificate includes a SAN for `localhost` and `127.0.0.1`.
