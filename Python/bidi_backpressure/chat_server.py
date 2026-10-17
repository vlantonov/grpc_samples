import logging
import time
from concurrent import futures

import grpc

import chat_pb2
import chat_pb2_grpc


class ChatService(chat_pb2_grpc.ChatServiceServicer):
    def Chat(self, request_iterator, context):
        for request in request_iterator:
            logging.info("server received %s (%d)", request.name, request.sequence)
            time.sleep(0.75)
            yield chat_pb2.ChatReply(
                message=f"processed: {request.name}",
                sequence=request.sequence,
            )


def serve(port: int = 50052):
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    chat_pb2_grpc.add_ChatServiceServicer_to_server(ChatService(), server)
    server.add_insecure_port(f"[::]:{port}")
    server.start()
    logging.info("Chat server listening on port %d", port)
    server.wait_for_termination()


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO, format="%(asctime)s %(levelname)s %(message)s")
    serve()
