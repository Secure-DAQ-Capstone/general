#include "relay.h"
#include "constants.h"

// Constructor definition
Relay::Relay(size_t max_buffer_size, const int receive_port, const char* receive_ip, bool debug, bool debug_sub) : Application(max_buffer_size, receive_port, receive_ip, debug, debug_sub){}


void Relay::relay_packet(const std::string &packet_str) {
    capstone_protobuf::EncryptedPacket encrypted_packet;
    
    // Parse the proto packet from a string
    bool success = get_encrypted_proto_packet(packet_str, encrypted_packet);

    if (success) {
        // Process the packet as needed



        if (this->debug) {
            std::cout << "Relaying packet: " << packet_str << std::endl;
        }
    } else {
        std::cerr << "Failed to parse and relay packet." << std::endl;
    }
}


void edit_packet_metadata(capstone_protobuf::EncryptedPacket &packet) {
    // Edit the packet metadata
    std::cout << "Edit the packet" << std::endl;
}




void Relay::update() {

    // Received UDP Protobuf Packets
    std::string packet_str = this->sub.read();
    
    // Relay the packet
    this->relay_packet(packet_str);
}



