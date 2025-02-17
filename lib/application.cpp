#include "application.h"
#include "udp_sub.h"
#include "constants.h"
#include "packet.pb.h"
#include <iostream>

// Constructor
Application::Application(size_t max_buffer_size, int receive_port, const char* receive_ip, bool debug, bool debug_sub)
    : debug(debug), sub(max_buffer_size, receive_port, receive_ip, debug_sub)
{
}


bool Application::get_proto_packet(std::string packet_str, capstone_protobuf::Packet &packet_output)
{
    capstone_protobuf::EncryptedPacket encrypted_packet;

    try
    {
        capstone_protobuf::Packet packet;

        // Parse the string. Raises an error if the input cannot be parsed.
        encrypted_packet.ParseFromString(packet_str);

        //Generate the nonce
        unsigned char nonce[crypto_secretbox_NONCEBYTES];
        symmetric_key_security_agent.generateNonce(nonce);
        std::string nonce_str(nonce, nonce+crypto_secretbox_NONCEBYTES);

        // Decrypt the payload
        std::string payload_str = decryptString(encrypted_packet.encrypted_payload(), nonce_str);

        // Copy the metadata from the encrypted packet to the new packet
        capstone_protobuf::MetaData *metadata_copy = new capstone_protobuf::MetaData();
        *metadata_copy = *encrypted_packet.mutable_metadata();

        // Set the metadata of the new packet
        packet_output.set_allocated_metadata(metadata_copy);
        
        // Make a new Payload object and copy the payload string to it
        capstone_protobuf::Packet::Payload *payload = new capstone_protobuf::Packet::Payload();
        payload->ParseFromString(payload_str);
        packet_output.set_allocated_payload(payload);

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

bool Application::get_encrypted_proto_packet(std::string packet_str, capstone_protobuf::EncryptedPacket &packet_output) 
{
    packet_output.ParseFromString(packet_str);

      // Debug logs
      if (this->debug)
      {
          std::cout << packet_output.DebugString() << std::endl;
      } 

    return false;
}


// Update the Application class
void Application::update() {
    // Received UDP Protobuf Packets
    std::string message = this->sub.read();
    
    // Convert the string into a proto packet
    capstone_protobuf::Packet packet;
    bool success = this->get_proto_packet(message, packet);
}

