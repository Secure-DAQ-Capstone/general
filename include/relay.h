#ifndef RELAY_H
#define RELAY_H

#include "base_subscriber.h"
#include "constants.h"
#include "udp_pub.h"

class Relay : public Base
{
public:
    Relay(const Config &config, bool debug, bool debug_sub, string board_id);

    void relay_packet(const std::string &packet_str);

    void update() override;

    void edit_packet_metadata(capstone_protobuf::EncryptedPacket &packet);

    void set_spoof_timestamp(bool spoof);

protected:
    UDPPub pub;
    std::string board_id;
    bool sabotage;
};

#endif // RELAY_H
