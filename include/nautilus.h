#ifndef NAUTILUS_H
#define NAUTILUS_H

#include <string>
#include "udp_sub.h"
#include "packet.pb.h"

class Nautilus {
public:
    Nautilus(bool debug);
    void update();

    capstone_protobuf::Packet get_proto_packet(std::string packet_str);

private:
    const bool UDP_DEBUG = true;
    UDPSub sub;
    bool debug;
};

#endif // NAUTILUS_H