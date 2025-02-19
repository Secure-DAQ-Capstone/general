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

    UDPSub(int port, const char *address, bool debug = false);

    std::string readPretty();

    // read data from the socket and return the string
    std::string read();

    ~UDPSub();

private:
    size_t max_buffer_size;
    int socket_fd;
    bool debug;

    std::string formatErrorMessage(const std::string &message) const
    {
        return "UDP-Sub: " + message;
    }
};

#endif // UDP_SUB