#include "relay.h"
#include <thread>
#include <atomic>
#include <iostream>


// Function to handle user input
void handle_user_input(Relay &relay, std::atomic<bool> &running)
{
    std::string command;
    while (running)
    {
        std::cin >> command;
        if (command == "sabotage")
        {
            relay.set_spoof_timestamp(true);
            std::cout << "Timestamp spoofing enabled." << std::endl;
        }
        else if (command == "flip")
        {
            relay.set_bitflip_bool(true);
            std::cout << "Bit Flipping enabled." << std::endl;
        }
        else if (command == "noflip")
        {
            relay.set_bitflip_bool(false);
            std::cout << "Bit Flipping disabled." << std::endl;
        }
        else if (command == "sigflip")
        {
            relay.set_flip_signature_bool(true);
            std::cout << "Digital Signature Bit Flipping enabled." << std::endl;
        }
        else if (command == "nosigflip")
        {
            relay.set_flip_signature_bool(false);
            std::cout << "Digital Signature Bit Flipping disabled." << std::endl;
        }
        else if (command == "stop")
        {
            relay.set_spoof_timestamp(false);
            std::cout << "Timestamp spoofing disabled." << std::endl;
        }
        else if (command == "exit")
        {
            running = false;
            std::cout << "Exiting..." << std::endl;
        }
    }
}
#include "arg_parser.h"
#include <string>
#include <iostream>

// Run the Relay class
int main(int argc, char *argv[])
{
    // Init protobuf variables
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Init Application
    bool debug_application = false;
    bool debug_sub = false;

    // Parse command line arguments
    ParsedArgs args = parseArguments(argc, argv);

    // Defaults for command line arguments
    int receive_port = PUBLISHER_PORT;
    int publish_port = PUBLISHER_PORT;

    Config config(receive_port, args.receive_ip, args.publish_ip, publish_port, args.machine_id);

    Relay relay(config, debug_application, debug_sub);

    std::atomic<bool> running(true);
    std::thread user_input_thread(handle_user_input, std::ref(relay), std::ref(running));

    // run the loop
    while (running)
    {
        relay.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    user_input_thread.join();

    return 0;
}