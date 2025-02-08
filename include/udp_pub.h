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

    UDPPub(size_t max_buffer_size, int port, in_addr_t address);

    void write(std::string message);

    ~UDPPub();

private:
    size_t max_buffer_size;
    sockaddr_in server_addr;
    int socket_fd;

    std::string formatErrorMessage(const std::string &message) const
    {
        return "UDPPub: " + message;
    }
};

#endif // UDP_PUB