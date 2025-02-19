#include "udp_sub.h"
#include "constants.h"

UDPSub::UDPSub(int port, const char* address, bool debug)
: debug(debug)
{
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0)
    {
        throw std::runtime_error(formatErrorMessage("Error opening Socket"));
    }
    this->max_buffer_size = UDP_BUFFER_SIZE;

    /**
     * Bind the socket to an address and port.
     * We expect this to bind to the computers local IP and port
     */
    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(address);

    if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        close(socket_fd);
        throw std::runtime_error(formatErrorMessage("Error binding Socket"));
    }
    else
    {
        std::cout << "Socket bound to address: " << inet_ntoa(server_addr.sin_addr) << " and port: " << ntohs(server_addr.sin_port) << std::endl;
    }
}


std::string UDPSub::read()
{
    // Receive message
    char buffer[this->max_buffer_size];

    /**
     * Define a socket address that will be populated with the senders address
     * by the recvfrom syscall
     */
    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    /**
     * Returns the number of bytes received.
    */
    ssize_t recv_bytes = recvfrom(socket_fd, buffer, sizeof(buffer) - 1, 0,
                                  (sockaddr *)&client_addr, &client_len);
    if (recv_bytes < 0)
    {
        throw std::runtime_error(formatErrorMessage("Error receiving message"));
    }

    /**
     *  create a string from the bytes received.
     */
    std::string received_message(buffer, recv_bytes);

    if (this->debug){
        std::cout << "Received message" << std::endl;
        std::cout << "Message: " << received_message << std::endl;
    }

    return received_message;
}


UDPSub::~UDPSub()
{
    close(socket_fd);
}