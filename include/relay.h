#ifndef RELAY_H
#define RELAY_H

#include "application.h"

class Relay : public Application {
public:
    Relay(bool debug, bool debug_sub);

    void relay_packet(const std::string &packet_str);

    void update() override;

    void edit_packet_metadata(capstone_protobuf::EncryptedPacket &packet);

};

#endif //RELAY_H




