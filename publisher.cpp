#include <iostream>
#include "packet.pb.h"
#include "udp-pub.cpp" // Include the UDPPub class

class Publisher {
public:
    Publisher(size_t max_buffer_size, int port, in_addr_t address)
        : udp_pub(max_buffer_size, port, address) {
        // Initialize the Protobuf library
        GOOGLE_PROTOBUF_VERIFY_VERSION;
    }

    ~Publisher() {
        // Clean up the Protobuf library
        google::protobuf::ShutdownProtobufLibrary();
    }

    void publish(int id, const std::string& data) {
         // Create a new packet
        tutorial::Packet packet;
        packet.set_board_id(board_id);
        *packet.mutable_time_data_read() = time_data_read;
        *packet.mutable_data() = data;
        packet.set_protocol(protocol);
        packet.set_original_message(original_message);
        packet.set_digital_signature(digital_signature);

        // Serialize the packet to a string
        std::string output;
        if (!packet.SerializeToString(&output)) {
            std::cerr << "Failed to serialize packet." << std::endl;
            return;
        }

        // Publish the packet using UDPPub
        udp_pub.write(output);
    }

    void runPublisher(size_t buffer_size, int port, in_addr_t address)
    {
        try
        {
            std::string message;
            while (true)
            {
                std::cout << "Enter message to send: ";
                std::getline(std::cin, message);
                std::cout << message << std::endl;
                udp_pub.write(message);
                std::cout << "Message sent: " << message << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Publisher Exception: " << e.what() << std::endl;
        }
    }

private:
    UDPPub udp_pub;
};

int main() {
    // Example usage of Publisher
    Publisher publisher(1024, 12345, INADDR_ANY);

    // Create a timestamp
    google::protobuf::Timestamp timestamp;
    timestamp.set_seconds(time(NULL));
    timestamp.set_nanos(0);

    // Create a data object
    tutorial::Packet::Data data;
    // Set data fields here

    publisher.publish(1, timestamp, data, tutorial::Packet::CAN, 

    return 0;
}