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

// Run the Relay class
int main(int argc, char *argv[])
{
    // Init protobuf variables
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Init Application
    bool debug_application = false;
    bool debug_sub = false;

    // defaults for command line arguments
    const char *receive_ip = "10.0.0.109";
    int receive_port = PUBLISHER_PORT;

    // Publisher
    const char *publish_ip = VisorLab::Tony;
    int publish_port = PUBLISHER_PORT;

    std::string board_id;
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

    // Parse command-line arguments
    if (argc > 2)
    {
        receive_ip = argv[2];
        std::cout << "Receive IP: " << receive_ip << std::endl;
    }
    if (argc > 3)
    {
        publish_ip = argv[3];
        std::cout << "Publish IP: " << publish_ip << std::endl;
    }


    Config config(receive_port, receive_ip, publish_ip, publish_port);

    Relay relay(config, debug_application, debug_sub, board_id);

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