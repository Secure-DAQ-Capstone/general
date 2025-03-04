#include "relay.h"
#include "arg_parser.h"
#include <string>
#include <iostream>

// Run the Relay class
int main(int argc, char *argv[])
{
    // Init protobuf variables
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Init Application
    bool debug_application = true;
    bool debug_sub = true;

    // Parse command line arguments
    ParsedArgs args = parseArguments(argc, argv);

     // Defaults for command line arguments
     int receive_port = PUBLISHER_PORT;
     int publish_port = PUBLISHER_PORT;

    Config config(receive_port, args.receive_ip, args.publish_ip, publish_port, args.machine_id);

    Relay relay(config, debug_application, debug_sub);

    // run the loop
    while (true)
    {
        relay.update();
    }

    return 0;
}