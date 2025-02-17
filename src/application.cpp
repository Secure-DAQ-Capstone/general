#include "application.h"
#include "proto_json_converter.h"
#include "mongodb_handler.h"
#include "udp_sub.h"
#include "constants.h"
#include "packet.pb.h"
#include <iostream>

// Constructor
Application::Application(const int receive_port, const char* receive_ip, bool debug, bool debug_sub)
    : debug(debug), sub(receive_port, receive_ip, LOOPBACK_IP, debug_sub)
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
    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    //Generate the nonce
    symmetric_key_security_agent.generateNonce(nonce);

    std::string nonce_str(nonce, nonce+crypto_secretbox_NONCEBYTES);

    std::string payload_str = decryptString(encrypted_packet.encrypted_payload(), nonce_str);
    capstone_protobuf::MetaData *metadata_copy = new capstone_protobuf::MetaData();
    *metadata_copy = *encrypted_packet.mutable_metadata();
    packet_output.set_allocated_metadata(metadata_copy);
    capstone_protobuf::Packet::Payload *payload = new capstone_protobuf::Packet::Payload();
    payload->ParseFromString(payload_str);
    packet_output.set_allocated_payload(payload);
    cout << packet_output.DebugString() << endl;
        

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