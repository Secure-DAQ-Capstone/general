#include "relay.h"

// Run the Relay class
int main(int argc, char *argv[])
{
    // Init protobuf variables
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Init Application
    bool debug_application = true;
    bool debug_sub = false;

    // defaults for command line arguments
    const char *receive_ip = GracesHouse::Ventana1_ETH1;
    int receive_port = PUBLISHER_PORT;

    // Parse command-line arguments
    if (argc > 1)
    {
        receive_ip = argv[1];
        std::cout << "Receive IP: " << receive_ip << std::endl;
    }
    if (argc > 2)
    {
        receive_port = std::stoi(argv[2]);
        std::cout << "Receive Port: " << receive_port << std::endl;
    }

    Relay relay(receive_port, receive_ip, debug_application, debug_sub);

    // run the loop
    while (true)
    {
        relay.update();
    }

    return 0;
}