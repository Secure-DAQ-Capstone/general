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

/**
 * Add the current board id to the relay chain
 */
void Relay::edit_packet_metadata(capstone_protobuf::EncryptedPacket &packet)
{
    // Create a new RelayChainEntry
    capstone_protobuf::MetaData::RelayChainEntry *entry = packet.mutable_metadata()->add_relay_chain();
    entry->set_board_id(board_id);                                    // Set the board ID (example value)
    entry->set_timestamp(static_cast<int32_t>(time(nullptr))); // Set the current timestamp

    // Debug log
    if (this->debug)
    {
        std::cout << "Added RelayChainEntry: board_id=" << entry->board_id() << ", timestamp=" << entry->timestamp() << std::endl;
    }
}

void Relay::update()
{

    // Received UDP Protobuf Packets
    std::string packet_str = this->sub.read();

    // Relay the packet
    this->relay_packet(packet_str);
}
