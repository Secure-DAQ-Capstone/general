#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "security.h"
#include <string>

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

struct VisorLab
{
    static constexpr const char *Ventana1 = "10.0.0.101";
    static constexpr const char *Ventana2 = "10.0.0.102";
    static constexpr const char *Ventana3 = "10.0.0.103";
    static constexpr const char *Ventana4 = "10.0.0.104";
    static constexpr const char *Tony = "10.0.0.1";
    static constexpr const char *FaresLaptop = "10.0.0.107";
    static constexpr const char *MattLaptop = "10.0.0.108";
};

struct MachineInfo {
    int number;
    std::string name;
    std::string id;

    MachineInfo(int num, const std::string& nm, const std::string& mid)
        : number(num), name(nm), id(mid) {}
};


struct MachineRegistry
{
    static const std::vector<MachineInfo> machines;

    static std::string getMachineId(int number) {
        for (const auto& machine: machines) {
            if (machine.number == number) {
                return machine.id;
            }
        }
        return "";
    }
};

// extern const std::vector<MachineInfo> MachineRegistry::machines;

static constexpr const char *LOOPBACK_IP = "127.0.0.1";

#define UDP_BUFFER_SIZE 500
#define PUBLISHER_PORT 8080

extern security_base security_agent;

struct Config
{
    int receive_port;
    const char *receive_ip;
    const char *publish_ip;
    int publish_port;
    std::string machine_id;

    Config(int r_port, const char *r_ip, const char *p_ip, int p_port, std::string machine_id)
        : receive_port(r_port), receive_ip(r_ip), publish_ip(p_ip), publish_port(p_port), machine_id(machine_id) {}
};

static char *homeDir = getenv("HOME");

// Declare a static global board ID for the nmea reader
extern std::string GLOBAL_BOARD_ID;

#define BOARD_ID_1 1
#define BOARD_ID_2 2
#define BOARD_ID_3 3
#define BOARD_ID_4 4

#endif // CONSTANTS_H
