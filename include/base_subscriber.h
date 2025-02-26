#ifndef BASE_H
#define BASE_H

#include <string>
#include "udp_sub.h"
#include "packet.pb.h"
#include "constants.h"
#include <vector>

using namespace std;

class Base
{
public:
    Base(const int receive_port, const char *receive_ip, bool debug, bool debug_sub);

    bool get_encrypted_proto_packet(std::string packet_str, capstone_protobuf::EncryptedPacket &packet_output);

    // Declare as virtual for the relay node
    virtual void update();

protected:
    bool debug;
    UDPSub sub;

private:
    // override with the inheriting class
    virtual std::string formatErrorMessage(const std::string &message) const
    {
        return "Base: " + message;
    }
};

#endif // BASE_H