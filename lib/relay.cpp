#include "relay.h"
#include "constants.h"

// Constructor definition
Relay::Relay(const Config &config, bool debug, bool debug_sub)
    : Base(config.receive_port, config.receive_ip, debug, debug_sub),
      pub(config.publish_port, config.publish_ip, debug_sub) {}

void Relay::relay_packet(const std::string &packet_str)
{
    capstone_protobuf::EncryptedPacket encrypted_packet;

    // Parse the proto packet from a string
    bool success = get_encrypted_proto_packet(packet_str, encrypted_packet);

    if (!success)
    {
        std::cerr << "Failed to parse and relay packet." << std::endl;
    }
    else
    {
        // Edit the packet metadata
        edit_packet_metadata(encrypted_packet);

        // Send the packet
        std::string packet_str;
        encrypted_packet.SerializeToString(&packet_str);
        pub.write(packet_str);
    }
}

void Relay::edit_packet_metadata(capstone_protobuf::EncryptedPacket &packet)
{
    // Edit the packet metadata
    std::cout << "Edit the packet" << std::endl;
}

void Relay::update()
{

    // Received UDP Protobuf Packets
    std::string packet_str = this->sub.read();

    // Relay the packet
    this->relay_packet(packet_str);
}
