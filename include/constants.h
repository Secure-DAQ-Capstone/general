#ifndef CONSTANTS_H
#define CONSTANTS_H

struct GraceHouse
{
    static constexpr const char *Ventana1 = "192.168.2.101";
    static constexpr const char *Ventana2 = "192.168.2.142";
    static constexpr const char *Ventana3 = "";
    static constexpr const char *Ventana4 = "";
};

static constexpr const char *LOOPBACK_IP = "127.0.0.1";

#define UDP_BUFFER_SIZE 500
#define PUBLISHER_PORT 8080

#endif // CONSTANTS_H
