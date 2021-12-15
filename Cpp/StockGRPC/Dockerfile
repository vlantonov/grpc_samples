FROM ubuntu:20.04 as build

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libgrpc++-dev \
    libprotobuf-dev \
    protobuf-compiler-grpc

ENV SAMPLE_BUILD_PATH /usr/local/grpc_sample

COPY . $SAMPLE_BUILD_PATH/src/grpc_sample/

RUN echo "-- building calculator" && \
    mkdir -p $SAMPLE_BUILD_PATH/out && \
    cd $SAMPLE_BUILD_PATH/out && \
    cmake -DCMAKE_BUILD_TYPE=Release $SAMPLE_BUILD_PATH/src/grpc_sample && \
    make install

RUN echo "greeter_server" > /usr/bin/start_server.sh && \
    chmod +x /usr/bin/start_server.sh && \
    echo "greeter_client" > /usr/bin/start_client.sh && \
    chmod +x /usr/bin/start_client.sh

WORKDIR $SAMPLE_BUILD_PATH
EXPOSE 50051
ENTRYPOINT ["/bin/bash", "/usr/bin/start_server.sh"]
CMD ["-s"]


