#include "application.h"
#include "udp_sub.h"
#include "constants.h"
#include "packet.pb.h"
#include <iostream>
#include <fstream>

// Constructor
Application::Application(int receive_port, const char *receive_ip, bool debug, bool debug_sub)
    : debug(debug), sub(receive_port, receive_ip, debug_sub)
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

        // Copy the metadata from the encrypted packet to the new packet
        capstone_protobuf::MetaData *metadata_copy = new capstone_protobuf::MetaData();
        *metadata_copy = *encrypted_packet.mutable_metadata();

        // Get the nonce from the metadata
        std::string nonce_str = metadata_copy->nonce();

        // Decrypt the payload
        std::string payload_str = decryptString(encrypted_packet.encrypted_payload(), nonce_str, GLOBAL_BOARD_ID);
        
        metadata_copy->set_decryption_succeeded(true);

        // Set the metadata of the new packet
        packet_output.set_allocated_metadata(metadata_copy);

        // Make a new Payload object and copy the payload string to it
        capstone_protobuf::Packet::Payload *payload = new capstone_protobuf::Packet::Payload();
        payload->ParseFromString(payload_str);
        packet_output.set_allocated_payload(payload);

        std::string signature_str = metadata_copy->digital_signature();
        string signature_board_id = metadata_copy->board_id_msg_origin();

        std::string str_payload;
        packet_output.payload().SerializeToString(&str_payload);

        bool verified_signature = verifyDigitalSignature(str_payload, signature_str, signature_board_id);

        packet_output.mutable_metadata()->set_signature_verified(verified_signature);
        // If the signature is not verified, log the packet
        if (!verified_signature)
        {
            std::string filename = "failed_messages.txt";
            std::ofstream file(filename, std::ios::app);

            if (!file.is_open())
            {
                std::cerr << "Failed to open file for appending: " << filename << std::endl;
                return false;
            }

            file << packet_output.DebugString() << std::endl;

            file.close();

        }
        else
        {
            //Add the public key to the metadata
            unsigned char *public_key = security_agent.getKey(signature_board_id);
            packet_output.mutable_metadata()->set_public_key(public_key, crypto_sign_PUBLICKEYBYTES);
        }

        // Debug logs
        if (this->debug)
        {
            std::cout << packet_output.DebugString() << std::endl;
        }

        return true;
    }
    catch (const std::exception &e)
    {
        std::string exception_message = e.what();
        if(exception_message.find("Decryption failed") != std::string::npos|| exception_message.find("Failed to open key file for type 0") != std::string::npos)
        {
            packet_output.set_allocated_metadata(encrypted_packet.release_metadata());
            capstone_protobuf::Packet::Payload *payload = new capstone_protobuf::Packet::Payload();
            payload->ParseFromString(encrypted_packet.encrypted_payload());
            packet_output.set_allocated_payload(payload);
            packet_output.mutable_metadata()->set_signature_verified(false);
            packet_output.mutable_metadata()->set_decryption_succeeded(false);
            return true;
        }
        else if (exception_message.find("Failed to open key file for type 2") != std::string::npos)
        {
            packet_output.mutable_metadata()->set_signature_verified(false);
            return true;
        }
        std::cerr << this->formatErrorMessage(e.what()) << '\n';
        return false;
    }

    return false;
}

bool Application::get_encrypted_proto_packet(std::string packet_str, capstone_protobuf::EncryptedPacket &packet_output)
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

// Update the Application class
void Application::update()
{
    // Received UDP Protobuf Packets
    std::string message = this->sub.read();

    // Convert the string into a proto packet
    capstone_protobuf::Packet packet;
    bool success = this->get_proto_packet(message, packet);
}
