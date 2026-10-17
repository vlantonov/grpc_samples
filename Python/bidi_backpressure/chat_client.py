import logging
import time

import grpc

import chat_pb2
import chat_pb2_grpc


def request_stream():
    for index in range(1, 11):
        yield chat_pb2.ChatRequest(name=f"client-{index}", sequence=index)
        time.sleep(0.2)


def run():
    with grpc.insecure_channel("localhost:50052") as channel:
        stub = chat_pb2_grpc.ChatServiceStub(channel)
        logging.info("sending requests to the bidirectional stream")

        for reply in stub.Chat(request_stream()):
            logging.info("received reply %d -> %s", reply.sequence, reply.message)


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO, format="%(asctime)s %(levelname)s %(message)s")
    run()
