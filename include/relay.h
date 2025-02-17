#include "application.h"

class Relay : public Application {
public:
    Relay(bool debug, bool debug_sub) : Application(debug, debug_sub) {}

    void relay_packet(const std::string &packet_str) {
        capstone_protobuf::Packet packet;
        if (get_proto_packet(packet_str, packet)) {
            // Process the packet as needed
            std::cout << "Relaying packet: " << json_data << std::endl;
            
        } else {
            std::cerr << "Failed to parse and relay packet." << std::endl;
        }
    }
};

