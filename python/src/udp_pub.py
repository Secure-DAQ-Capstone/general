import socket

class UDPPub:
    def __init__(self, max_buffer_size, port, address, broadcast=False, debug=False):
        self.broadcast_enable = broadcast
        self.debug = debug
        self.max_buffer_size = max_buffer_size

        self.socket_fd = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        if self.socket_fd < 0:
            raise RuntimeError("Error opening Socket")

        if broadcast:
            raise NotImplementedError("broadcast is not successfully tested yet")
            self.socket_fd.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)

        self.dest_addr = (address, port)

        if debug:
            print(f"Sending messages to address: {address} and port: {port}")

    def write(self, message, debug=False):
        sent_bytes = self.socket_fd.sendto(message.encode(), self.dest_addr)
        if sent_bytes < 0:
            raise RuntimeError("Error sending message")
        elif debug:
            print(f"Sent {sent_bytes} bytes")

    def __del__(self):
        self.socket_fd.close()