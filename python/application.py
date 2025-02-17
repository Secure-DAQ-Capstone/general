import json
import socket
from google.protobuf.message import DecodeError
from packet_pb2 import EncryptedPacket, Packet, MetaData
from proto_json_converter import ProtoJsonConverter
from mongodb_handler import MongoDBHandler
from constants import UDP_BUFFER_SIZE, PUBLISHER_PORT, LOOPBACK_IP

class Application:
    def __init__(self, debug, debug_sub):
        self.debug = debug
        self.sub = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sub.bind((LOOPBACK_IP, PUBLISHER_PORT))
        self.sub.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, UDP_BUFFER_SIZE)
        self.debug_sub = debug_sub

    def decrypt_string(self, encrypted_payload):
        # Implement the decryption logic here
        return encrypted_payload  # Placeholder

    def get_proto_packet(self, packet_str):
        encrypted_packet = EncryptedPacket()
        packet_output = Packet()

        try:
            # Parse the string. Raises an error if the input cannot be parsed
            encrypted_packet.ParseFromString(packet_str)

            # Decrypt the payload
            payload_str = self.decrypt_string(encrypted_packet.encrypted_payload)

            # Make a new MetaData object
            metadata_copy = MetaData()

            # Copy the metadata from the encrypted packet to the new packet
            metadata_copy.CopyFrom(encrypted_packet.metadata)

            # Set the metadata of the new packet
            packet_output.metadata.CopyFrom(metadata_copy)

            # Make a new Payload object and copy the payload string to it
            payload = Packet.Payload()
            payload.ParseFromString(payload_str)

            # Set the payload of the new packet
            packet_output.payload.CopyFrom(payload)

            if self.debug:
                print(packet_output)

            return True, packet_output
        except DecodeError as e:
            print(f"Error: {e}")
            return False, None

    def update(self):
        # Received UDP Protobuf Packets
        message, _ = self.sub.recvfrom(UDP_BUFFER_SIZE)

        # Convert the string into a proto packet
        success, packet = self.get_proto_packet(message)

        if success:
            # Convert packet to JSON
            json_data = ProtoJsonConverter.to_json(packet)

            # Store JSON data into MongoDB
            db_handler = MongoDBHandler("mongodb://localhost:27017", "dataMarineSystem", "packetData")
            db_handler.store_json(json_data)

if __name__ == "__main__":
    # Init protobuf variables
    # GOOGLE_PROTOBUF_VERIFY_VERSION is not needed in Python

    # initialize the classes
    debug_application = True
    debug_sub = False
    application = Application(debug_application, debug_sub)

    # run the loop
    while True:
        application.update()