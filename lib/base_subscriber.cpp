#include "base_subscriber.h"
#include "udp_sub.h"
#include "constants.h"
#include "packet.pb.h"
#include <iostream>

// Constructor
Base::Base(int receive_port, const char *receive_ip, bool debug, bool debug_sub)
    : debug(debug), sub(receive_port, receive_ip, debug_sub)
{
}

bool Base::get_encrypted_proto_packet(std::string packet_str, capstone_protobuf::EncryptedPacket &packet_output)
{
    try
    {
        packet_output.ParseFromString(packet_str);

        // Debug logs
        if (this->debug)
        {
            std::cout << packet_output.DebugString() << std::endl;
        }
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << this->formatErrorMessage(e.what()) << '\n';
        return false;
    }

    return false;
}

// Update the Base class
void Base::update()
{
    // Received UDP Protobuf Packets
    std::string message = this->sub.read();

    // Convert the string into a proto packet
    capstone_protobuf::EncryptedPacket packet;
    bool success = this->get_encrypted_proto_packet(message, packet);
}
