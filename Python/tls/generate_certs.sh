#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")"
mkdir -p certs

CA_KEY="certs/ca.key"
CA_CERT="certs/ca.crt"
SERVER_KEY="certs/server.key"
SERVER_CSR="certs/server.csr"
SERVER_CERT="certs/server.crt"
CLIENT_KEY="certs/client.key"
CLIENT_CSR="certs/client.csr"
CLIENT_CERT="certs/client.crt"

openssl req -x509 -newkey rsa:2048 -keyout "$CA_KEY" -out "$CA_CERT" -days 3650 -nodes -subj "/CN=grpc-samples-ca"

openssl req -new -newkey rsa:2048 -keyout "$SERVER_KEY" -out "$SERVER_CSR" -nodes -subj "/CN=localhost"
cat > certs/server.ext <<'EOF'
subjectAltName=DNS:localhost,IP:127.0.0.1
extendedKeyUsage=serverAuth
keyUsage=digitalSignature,keyEncipherment
EOF
openssl x509 -req -in "$SERVER_CSR" -CA "$CA_CERT" -CAkey "$CA_KEY" -CAcreateserial -out "$SERVER_CERT" -days 365 -extfile certs/server.ext

openssl req -new -newkey rsa:2048 -keyout "$CLIENT_KEY" -out "$CLIENT_CSR" -nodes -subj "/CN=grpc-client"
cat > certs/client.ext <<'EOF'
extendedKeyUsage=clientAuth
keyUsage=digitalSignature,keyEncipherment
EOF
openssl x509 -req -in "$CLIENT_CSR" -CA "$CA_CERT" -CAkey "$CA_KEY" -CAcreateserial -out "$CLIENT_CERT" -days 365 -extfile certs/client.ext

rm -f "$SERVER_CSR" "$CLIENT_CSR" certs/server.ext certs/client.ext certs/*.srl
chmod 600 "$CA_KEY" "$SERVER_KEY" "$CLIENT_KEY"

printf '\nCreated certificates in %s\n' "$(pwd)/certs"
