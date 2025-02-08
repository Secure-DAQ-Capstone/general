#ifndef UDP_SUB
#define UDP_SUB

#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

class UDPSub
{
public:
    UDPSub() = delete; // prevent usage of default constructor

    UDPSub(size_t max_buffer_size, int port, in_addr_t address);

    std::string read();
    ~UDPSub();

private:
    size_t max_buffer_size;
    int socket_fd;

    std::string formatErrorMessage(const std::string &message) const
    {
        return "UDP-Sub: " + message;
    }
};

#endif // UDP_SUB