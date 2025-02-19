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

    // Personals
    static constexpr const char *GraceLaptop = "192.168.2.56";
    static constexpr const char *MattLinux = "192.168.2.137";
};

struct MattsHouse
{
    static constexpr const char *BROADCAST_IP = "192.168.2.255";
    static constexpr int BROADCAST_PORT = 12345;
};

static constexpr const char *LOOPBACK_IP = "192.168.2.56";

#define UDP_BUFFER_SIZE 500
#define PUBLISHER_PORT 8080

extern security_base symmetric_key_security_agent;
struct Config
{
    int receive_port;
    const char *receive_ip;
    const char *publish_ip;
    int publish_port;

    Config(int r_port = 12345, const char *r_ip = LOOPBACK_IP, const char *p_ip = LOOPBACK_IP, int p_port = 12346)
        : receive_port(r_port), receive_ip(r_ip), publish_ip(p_ip), publish_port(p_port) {}
};

#endif // CONSTANTS_H
