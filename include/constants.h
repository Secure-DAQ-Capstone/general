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

struct Config
{
    int receive_port;
    const char *receive_ip;
    const char *publish_ip;
    int publish_port;

    Config(int r_port = 12345, const char *r_ip = LOOPBACK_IP, const char *p_ip = LOOPBACK_IP, int p_port = 12346)
        : receive_port(r_port), receive_ip(r_ip), publish_ip(p_ip), publish_port(p_port) {}
};

static char* homeDir = getenv("HOME");

#define BOARD_ID_1 1
#define BOARD_ID_2 2
#define BOARD_ID_3 3
#define BOARD_ID_4 4

#endif // CONSTANTS_H
