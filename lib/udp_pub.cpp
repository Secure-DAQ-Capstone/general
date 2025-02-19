#include "udp_pub.h"
#include "constants.h"

UDPPub::UDPPub(int port, const char* address, bool broadcast, bool debug)
{
    this->broadcastEnable = broadcast;
    this->debug = debug;
    in_addr_t sendto_address = inet_addr(address); // ventana2

    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0)
    {
        throw std::runtime_error(formatErrorMessage("Error opening Socket"));
    }
    this->max_buffer_size = UDP_BUFFER_SIZE;

    /**
     * Started to implement broadcast, but it is not tested yet.
     * I don't know if a special kind of network is required for it to work properly
     * (i.e. does a wifi router block broadcasted messages).
     */
    if (broadcast)
    {
        throw std::logic_error(formatErrorMessage("broadcast is not successfully tested yet"));
        int broadcastEnable = 1;
        if (setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) < 0)
        {
            throw std::runtime_error(formatErrorMessage("Error setting socket option for broadcast"));
        }
    }

    /**
     * Define the address that we want to send too.
     * This is saved as a private variable.
     *
     * If we are broadcasting, the ip address should be set to the computer's broadcast address.
     *
     */
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    dest_addr.sin_addr.s_addr = sendto_address;

    if (debug){
        std::cout << "Sending messages to address: " << inet_ntoa(dest_addr.sin_addr) << " and port: " << ntohs(dest_addr.sin_port) << std::endl;
    }
    
}

void UDPPub::write(std::string message, bool debug)
{
    /**
     * The message is converted from a std::string to a c string. 
     * the c_str() function returns a pointer to a null terminated char array.
    */
    // Send the message to the server
    ssize_t sent_bytes = sendto(socket_fd, message.c_str(), message.size(), 0,
                                (sockaddr *)&dest_addr, sizeof(dest_addr));
    if (sent_bytes < 0)
    {
        throw std::runtime_error(formatErrorMessage("Error sending message"));
    } 
    else if (debug)
    {
        std::cout << "Sent " << sent_bytes << std::endl;
    }
}

UDPPub::~UDPPub()
{
    close(socket_fd);
}