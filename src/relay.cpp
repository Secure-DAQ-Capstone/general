#include "relay.h"

// Constructor definition
Relay::Relay(const int receive_port, const char* receive_ip, bool debug, bool debug_sub) : Application(receive_port, receive_ip, debug, debug_sub) {}


void Relay::relay_packet(const std::string &packet_str) {
    capstone_protobuf::EncryptedPacket encrypted_packet;
    
    // Parse the proto packet from a string
    bool success = get_encrypted_proto_packet(packet_str, &encrypted_packet);

    if (success) {
        // Process the packet as needed



        if (this->debug_sub) {
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
    // Receive a packet
    std::string packet_str = this->receive_packet();

    // Relay the packet
    this->relay_packet(packet_str);
    
}



// Run the Relay class
int main()
{
    // Init protobuf variables
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Init Application
    bool debug_application = true;
    bool debug_sub = false;
    const int receive_port = PUBLISHER_PORT;
    Relay relay(receive_port, LOOPBACK_IP, debug_application, debug_sub)

    // run the loop
    while (true)
    {
        relay.update();
    }

    return 0;
}