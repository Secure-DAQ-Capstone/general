#include "mongo_app.h"
#include <iostream>

// Constructor definition
MongoApp::MongoApp(const int receive_port, const char *receive_ip, bool debug, bool debug_sub)
    : Application(receive_port, receive_ip, debug, debug_sub)
{
    if (debug)
    {
        std::cout << "MongoDB App initialized" << std::endl;
    }
}

// Override the update method
void MongoApp::update()
{

    // Received UDP Protobuf Packets
    std::string message = this->sub.read();

    // Convert the string into a proto packet
    capstone_protobuf::Packet packet;
    bool success = this->get_proto_packet(message, packet);

    if (success)
    {
        // Convert packet to JSON
        std::string json_data = ProtoJsonConverter::toJson(packet);

        // Store JSON data into MongoDB
        MongoDBHandler dbHandler("mongodb://localhost:27017", "dataMarineSystem", "data");
        dbHandler.storeJson(json_data);
    }
}

// Run the Application class
int main(int argc, char *argv[])
{
    // Init protobuf variables
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <board_id>" << std::endl;
        return 1;
    }
    else
    {
        //Get the board id
        board_id = argv[1];
    }

    // Init Application
    bool debug_application = true;
    bool debug_sub = false;
    size_t max_buffer_size = UDP_BUFFER_SIZE;
    MongoApp mongo_app(PUBLISHER_PORT, "10.0.0.109", debug_application, debug_sub);

    // run the loop
    while (true)
    {
        mongo_app.update();
    }

    return 0;
}
