#include "nautilus.h"
#include "udp_sub.h"
#include "constants.h"
#include "packet.pb.h"
#include <iostream>

// Constructor
Nautilus::Nautilus(bool debug)
    : debug(debug), sub(UDP_BUFFER_SIZE, PUBLISHER_PORT, GraceHouse::Ventana1, debug) 
    {  
}

capstone_protobuf::Packet Nautilus::get_proto_packet(std::string packet_str) {
    
    capstone_protobuf::Packet packet;
    std::string deserialized_packet;

    // Temp Derserializing Packet Example
    if (encrypted_packet.ParseFromString(packet_str)) {
      capstone_protobuf::Packet packet;
      std::string payload_str = decryptString(encrypted_packet.encrypted_payload());
      capstone_protobuf::MetaData *metadata_copy = new capstone_protobuf::MetaData();
      *metadata_copy = *encrypted_packet.mutable_metadata();
      packet.set_allocated_metadata(metadata_copy);
      capstone_protobuf::Packet::Payload *payload = new capstone_protobuf::Packet::Payload();
      payload->ParseFromString(payload_str);
      packet.set_allocated_payload(payload);
      std::cout << packet.DebugString() << std::endl;
  
    } else {
        std::cerr << "Failed to parse string into Protobuf message!" << std::endl;
    }

    return packet;
}

// Update the Nautilus class
void Nautilus::update() {

    // Received UDP Protobuf Packets
    std::string message = this->sub.read();
    if (this->debug) {
        std::cout << "Received message: " << message << std::endl;
    }

    // Convert the string into a proto packet
    capstone_protobuf::Packet packet = this->get_proto_packet(message);
    if (this->debug) {
        std::cout << "Deserialized message: " << deserialized_packet << std::endl;
    }
}


// Run the Nautilus class
int main() {
    
    // initialize the classes
    Nautilus nautilus(true);

    // run the loop
    while (true) {
        nautilus.update();
    }

    return 0;
}