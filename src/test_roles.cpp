#include "udp_pub.h"
#include "udp_sub.h"
#include "constants.h"
#include <iostream>
#include <thread>

void runSubscriber(int port, const char *address)
{
    try
    {
        UDPSub subscriber(port, address);
        while (true)
        {
            std::string message = subscriber.read();
            std::cout << "Received: " << message << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Subscriber Exception: " << e.what() << std::endl;
    }
}

void runPublisher(int port, const char *address, bool enable_broadcast, bool debug)
{
    try
    {
        UDPPub publisher(port, address, enable_broadcast, debug);
        std::string message;
        while (true)
        {
            std::cout << "Enter message to send: ";
            std::getline(std::cin, message);
            std::cout << message << std::endl;
            publisher.write(message);
            std::cout << "Message sent: " << message << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Publisher Exception: " << e.what() << std::endl;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <pub|sub>" << std::endl;
        return 1;
    }

    size_t buffer_size = 1024;
    int port = 8080;

    /**
     * Grace's House
     * 192.168.2.142   ventana2
     * 192.168.2.101   ventana1
     */

    /**
     * This is the port that te publisher would send messages too.
     * It doesn't bind to a socket, it just sends a message when needed.
    //  *
     * Therefore, this would need to be replaced with the address of the computer you want to send too.
     */
    // in_addr_t sendto_address = inet_addr("192.168.2.142"); // ventana2
    // in_addr_t sendto_address = inet_addr("192.168.2.101"); // ventana1

    /**
     * The subscriber is binding a socket to it's local IP and Port.
     * Therefore you can expect this to be a loop back.
     */
    const char *sendto_address = "192.168.2.101"; // ventana2
    // const char* sub_local_address = "127.0.0.1";
    const char *sub_local_address = GracesHouse::Ventana1_ETH1; // ventana1

    std::string role = argv[1];
    if (role == "sub")
    {
        runSubscriber(port, sub_local_address);
    }
    else if (role == "pub")
    {
        runPublisher(port, sendto_address, false, true);
    }
    else
    {
        std::cerr << "Invalid role: " << role << std::endl;
        std::cerr << "Usage: " << argv[0] << " <pub|sub>" << std::endl;
        return 1;
    }

    return 0;
}