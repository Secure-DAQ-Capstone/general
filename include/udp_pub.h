#ifndef UDP_PUB
#define UDP_PUB

#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

class UDPPub
{
public:
    UDPPub() = delete; // prevent usage of default constructor

    UDPPub(int port, const char *address, bool debug = false, bool broadcast = false);

    void write(std::string message, bool debug = false);

    ~UDPPub();

private:
    size_t max_buffer_size;
    sockaddr_in dest_addr;
    int socket_fd;
    bool debug;
    bool broadcastEnable;

    std::string formatErrorMessage(const std::string &message) const
    {
        return "UDPPub: " + message;
    };
};

#endif // UDP_PUB