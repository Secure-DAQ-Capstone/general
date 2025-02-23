#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "security.h"

struct GracesHouse
{
    static constexpr const char *Ventana1_ETH1 = "192.168.2.101";
    static constexpr const char *Ventana2 = "192.168.2.142";
    static constexpr const char *Ventana3 = "";
    static constexpr const char *Ventana4 = "";
    static constexpr const char *BROADCAST_IP = "";
    static constexpr const char *GraceLaptop = "192.168.2.56";
    static constexpr const char *FaresLaptop = "192.168.2.127";
};

struct MattsHouse
{
    static constexpr const char *BROADCAST_IP = "192.168.2.255";
    static constexpr int BROADCAST_PORT = 12345;
};

static constexpr const char *LOOPBACK_IP = "127.0.0.1";

#define UDP_BUFFER_SIZE 500
#define PUBLISHER_PORT 8080

extern security_base symmetric_key_security_agent;
extern security_base signature_verifier_security_agent;

#endif // CONSTANTS_H
