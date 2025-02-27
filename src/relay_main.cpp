#include "relay.h"

// Run the Relay class
int main(int argc, char *argv[])
{
    // Init protobuf variables
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Init Application
    bool debug_application = true;
    bool debug_sub = true;

    // defaults for command line arguments
    const char *receive_ip = VisorLab::Ventana1;
    int receive_port = PUBLISHER_PORT;

    // Publisher
    const char *publish_ip = VisorLab::Tony;
    int publish_port = PUBLISHER_PORT;


    // Parse command-line arguments
    if (argc > 1)
    {
        receive_ip = argv[1];
        std::cout << "Receive IP: " << receive_ip << std::endl;
    }
    if (argc > 2)
    {
        publish_ip = argv[2];
        std::cout << "Publish IP: " << publish_ip << std::endl;
    }


    Config config(receive_port, receive_ip, publish_ip, publish_port);

    Relay relay(config, debug_application, debug_sub);

    // run the loop
    while (true)
    {
        relay.update();
    }

    return 0;
}