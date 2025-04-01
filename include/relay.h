#ifndef RELAY_H
#define RELAY_H

#include "base_subscriber.h"
#include "constants.h"
#include "udp_pub.h"
#include <string>

class Relay : public Base
{
public:
    Relay(const Config &config, bool debug, bool debug_sub);

    void relay_packet(const std::string &packet_str);

    void update() override;

    void edit_packet_metadata(capstone_protobuf::EncryptedPacket &packet);

    void set_spoof_timestamp(bool spoof);

    void set_bitflip_bool(bool flip);

    void set_flip_signature_bool(bool sig_flip);

protected:
    UDPPub pub;
    std::string board_id;
    bool sabotage;
    bool bit_flip;
    bool flip_signature;
};

#endif // RELAY_H
