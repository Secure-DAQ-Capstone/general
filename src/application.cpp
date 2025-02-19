#include "application.h"
#include "proto_json_converter.h"
#include "mongodb_handler.h"
#include "udp_sub.h"
#include "constants.h"
#include "packet.pb.h"
#include <iostream>
#include <fstream>

// Constructor
Application::Application(bool debug, bool debug_sub)
    : debug(debug), sub(UDP_BUFFER_SIZE, PUBLISHER_PORT, LOOPBACK_IP, debug_sub)
{
}

/**
 * the packet_out is where the completed packet will be stored.
 */
bool Application::get_proto_packet(std::string packet_str, capstone_protobuf::Packet &packet_output)
{
    capstone_protobuf::EncryptedPacket encrypted_packet;

    try
    {
        capstone_protobuf::Packet packet;
        // Parse the string. Raises an error if the input cannot be parses
        encrypted_packet.ParseFromString(packet_str);
        //std::cout << encrypted_packet.DebugString() << std::endl;

        // // Decrypt the payload
         //std::string payload_str = decryptString(encrypted_packet.encrypted_payload());
    // unsigned char nonce[crypto_secretbox_NONCEBYTES];
    //Generate the nonce
    // symmetric_key_security_agent.generateNonce(nonce);

    capstone_protobuf::MetaData *metadata_copy = new capstone_protobuf::MetaData();
    *metadata_copy = *encrypted_packet.mutable_metadata();
    
    std::string nonce_str = metadata_copy->nonce();

    std::string payload_str = decryptString(encrypted_packet.encrypted_payload(), nonce_str);
    
    packet_output.set_allocated_metadata(metadata_copy);
    capstone_protobuf::Packet::Payload *payload = new capstone_protobuf::Packet::Payload();
    payload->ParseFromString(payload_str);
    packet_output.set_allocated_payload(payload);
    cout << packet_output.DebugString() << endl;
        // // Make a new MetaData object
        // capstone_protobuf::MetaData *metadata_copy = new capstone_protobuf::MetaData();

        // // Copy the metadata from the encrypted packet to the new packet
        // *metadata_copy = *encrypted_packet.mutable_metadata();

        // // Set the metadata of the new packet
        // packet_output.set_allocated_metadata(metadata_copy);

        // // Make a new Payload object and copy the payload string to it
        // capstone_protobuf::Packet::Payload *payload = new capstone_protobuf::Packet::Payload();
        // payload->ParseFromString(payload_str);

        // // Set the payload of the our new packet
        // packet_output.set_allocated_payload(payload);

        // if (this->debug)
        // {
        //     std::cout << packet_output.DebugString() << std::endl;
        // }

        std::string signature_str = metadata_copy->digital_signature();

        std::string str_payload;
        packet_output.payload().SerializeToString(&str_payload);

        bool verified_signature = verifyDigitalSignature(str_payload, signature_str);

        //If the signature is not verified, log the packet
        if(!verified_signature)
        {
            std::string filename = "failed_messages.txt";
            std::ofstream file(filename, std::ios::app);

            if (!file.is_open()) {
                std::cerr << "Failed to open file for appending: " << filename << std::endl;
                return false;
            }

            file << packet_output.DebugString() << std::endl;

            file.close();
        }
        

        return verified_signature;
    }
    catch (const std::exception &e)
    {
        std::cerr << this->formatErrorMessage(e.what()) << '\n';
        return false;
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
    
    if (success) {
        // Convert packet to JSON
        std::string json_data = ProtoJsonConverter::toJson(packet);
        
        // Store JSON data into MongoDB
        MongoDBHandler dbHandler("mongodb://localhost:27017", "dataMarineSystem", "data");
        dbHandler.storeJson(json_data);
    }
}

// Run the Application class
int main()
{
    // Init protobuf variables
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // initialize the classes
    bool debug_application = true;
    bool debug_sub = false;
    Application application(debug_application, debug_sub);

    // run the loop
    while (true)
    {
        application.update();
    }

    return 0;
}