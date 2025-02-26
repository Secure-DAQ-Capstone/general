#ifndef RELAY_H
#define RELAY_H

#include "base_subscriber.h"
#include "constants.h"
#include "udp_pub.h"

class Relay : public Base
{
public:
    Relay(const Config &config, bool debug, bool debug_sub);

    void relay_packet(const std::string &packet_str);

    void update() override;

    void edit_packet_metadata(capstone_protobuf::EncryptedPacket &packet);

protected:
    UDPPub pub;
};

#endif // RELAY_H
