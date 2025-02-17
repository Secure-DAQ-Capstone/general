#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include "udp_sub.h"
#include "packet.pb.h"
#include "security.h"
#include <vector>

using namespace std;
security_base symmetric_key_security_agent("../symmetric_key_boards.txt");
class Application
{
public:
    Application(const int receive_port, const char* receive_ip, bool debug, bool debug_sub);
    
    // Declare as virtual for the relay node
    virtual void update(); 

    // Get a decrypted packet from the UDPSub. The packet_out is where the completed packet will be stored.
    bool get_proto_packet(std::string packet_str, capstone_protobuf::Packet &packet_output);

    // Get an encrypted packet from the UDPSub
    bool get_encrypted_proto_packet(std::string packet_str, capstone_protobuf::EncryptedPacket &packet_output);

    std::string decryptString(std::string str, std::string nonce_str)
    {
        unsigned char nonce[crypto_secretbox_NONCEBYTES];
        
        //Convert the nonce string to a char array
        copy(nonce_str.begin(), nonce_str.end(), nonce);

        //Decrypt the data
        vector<unsigned char> decrypted_array(str.begin(), str.end());
        vector<unsigned char> decrypted = symmetric_key_security_agent.decrypt(decrypted_array, str.length(), nonce);
        string decrypted_str(decrypted.begin(), decrypted.end());

        return decrypted_str;
    }
    
private:
    const bool UDP_DEBUG = true;
    UDPSub sub;
    bool debug;

    std::string formatErrorMessage(const std::string &message) const
    {
        return "UDPPub: " + message;
    }
};

#endif // APPLICATION_H