#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include "udp_sub.h"
#include "packet.pb.h"

class Application
{
public:
    Application(bool debug, bool debug_sub);
    void update();

    bool get_proto_packet(std::string packet_str, capstone_protobuf::Packet &packet_output);

    std::string decryptString(std::string str)
    {
        return str;
    }

private:
    const bool UDP_DEBUG = true;
    UDPSub sub;
    bool debug;

    std::string formatErrorMessage(const std::string &message) const
    {
        return "UDPPub: " + message;
    }
};

#endif // APPLICATION_H