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

    UDPSub(size_t max_buffer_size, int port, in_addr_t address)
    {
        socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (socket_fd < 0)
        {
            throw std::runtime_error(formatErrorMessage("Error opening Socket"));
        }
        this->max_buffer_size = max_buffer_size;
        // Bind the socket to an address and port
        sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        server_addr.sin_addr.s_addr = address;

        if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        {
            close(socket_fd);
            throw std::runtime_error(formatErrorMessage("Error binding Socket"));
        }
    }

    std::string read()
    {
        // Receive message
        char buffer[max_buffer_size];
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        ssize_t recv_bytes = recvfrom(socket_fd, buffer, sizeof(buffer) - 1, 0,
                                      (sockaddr *)&client_addr, &client_len);
        if (recv_bytes < 0)
        {
            throw std::runtime_error(formatErrorMessage("Error receiving message"));
        }
        buffer[recv_bytes] = '\0';
        std::string received_message(buffer, recv_bytes);
        std::cout << "Received message" << std::endl;
        return received_message;
    }
    ~UDPSub()
    {
        close(socket_fd);
    }

private:
    size_t max_buffer_size;
    int socket_fd;

    std::string formatErrorMessage(const std::string &message) const
    {
        return "UDP-Sub: " + message;
    }
};

#endif // UDP_SUB