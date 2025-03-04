#include "arg_parser.h"

ParsedArgs parseArguments(int argc, char *argv[]) {
    ParsedArgs args;
    args.receive_ip = VisorLab::Ventana1;
    args.publish_ip = VisorLab::Tony;

    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <machine_number> [receive_ip] [publish_ip]" << std::endl;
        std::cout << "  <machine_number>: The number corresponding to the machine ID (e.g., 1 for ventana1)" << std::endl;
        std::cout << "  [receive_ip]: (Optional) The IP address to receive data from" << std::endl;
        std::cout << "  [publish_ip]: (Optional) The IP address to publish data to" << std::endl;
        std::cout << "Example: " << argv[0] << " 1 10.0.0.101 10.0.0.102" << std::endl;
        exit(1);
    } else {
        // Get the machine ID based on the number
        int machine_number = std::stoi(argv[1]);
        args.machine_id = MachineRegistry::getMachineId(machine_number);

        if (args.machine_id.empty()) {
            std::cerr << "Invalid machine number: " << machine_number << std::endl;
            exit(1);
        } 
        else 
        {
            std::cout << "Machine ID: " << args.machine_id << std::endl;
        }
    }

    // Parse command-line arguments
    if (argc > 2) {
        args.receive_ip = argv[2];
        std::cout << "Receive IP: " << args.receive_ip << std::endl;
    }
    if (argc > 3) {
        args.publish_ip = argv[3];
        std::cout << "Publish IP: " << args.publish_ip << std::endl;
    }

    return args;
}