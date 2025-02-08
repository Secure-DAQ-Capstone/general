#include "udp-pub.cpp"
#include "udp-sub.cpp"

int main()
{
    try
    {
        // Define the address and port
        in_addr_t address = inet_addr("127.0.0.1");
        int port = 8080;
        size_t buffer_size = 1024;

        // Create publisher and subscriber
        UDPPub publisher(buffer_size, port, address);
        UDPSub subscriber(buffer_size, port, address);

        // Send a message
        std::string message = "Hello, UDP!";
        publisher.write(message);
        std::cout << "Message sent: " << message << std::endl;

        // Receive the message
        std::string received_message = subscriber.read();
        std::cout << "Message received: " << received_message << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}