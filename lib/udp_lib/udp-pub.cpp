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

    UDPPub(size_t max_buffer_size, int port, in_addr_t address)
    {
        socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (socket_fd < 0)
        {
            throw std::runtime_error(formatErrorMessage("Error opening Socket"));
        }
        this->max_buffer_size = max_buffer_size;

        // Bind the socket to an address and port
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        server_addr.sin_addr.s_addr = address;
    }

    void write(std::string message)
    {
        // Send the message to the server
        ssize_t sent_bytes = sendto(socket_fd, message.c_str(), message.size(), 0,
                                    (sockaddr *)&server_addr, sizeof(server_addr));
        if (sent_bytes < 0)
        {
            throw std::runtime_error(formatErrorMessage("Error sending message"));
        }
    }

    ~UDPPub()
    {
        close(socket_fd);
    }

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