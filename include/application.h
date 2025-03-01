#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include "udp_sub.h"
#include "packet.pb.h"
#include "security.h"
#include "constants.h"
#include <vector>

using namespace std;

class Application
{
public:
    Application(const int receive_port, const char *receive_ip, bool debug, bool debug_sub);

    // Declare as virtual for the relay node
    virtual void update();

    // Get a decrypted packet from the UDPSub. The packet_out is where the completed packet will be stored.
    bool get_proto_packet(std::string packet_str, capstone_protobuf::Packet &packet_output);

    // Get an encrypted packet from the UDPSub
    bool get_encrypted_proto_packet(std::string packet_str, capstone_protobuf::EncryptedPacket &packet_output);

    std::string decryptString(std::string str, std::string nonce_str, int board_id = 2)
    {
        unsigned char nonce[crypto_secretbox_NONCEBYTES];

        // Convert the nonce string to a char array
        copy(nonce_str.begin(), nonce_str.end(), nonce);

        // Decrypt the data
        vector<unsigned char> decrypted_array(str.begin(), str.end());
        // Should be defined globally
        vector<unsigned char> decrypted = security_agent.decrypt(decrypted_array, str.length(), nonce, board_id);
        string decrypted_str(decrypted.begin(), decrypted.end());

        return decrypted_str;
    }

    bool verifyDigitalSignature(std::string data, std::string signature, int board_id = 2)
    {

        unsigned char sig[crypto_sign_BYTES];

        // Conver the signature string into an array of unsigned characters
        copy(signature.begin(), signature.end(), sig);

        bool verified = security_agent.verifySignature(sig, (unsigned char *)data.data(), data.length(), board_id);

        return verified;
    }

protected:
    bool debug;
    UDPSub sub;

private:
    std::string formatErrorMessage(const std::string &message) const
    {
        return "Application: " + message;
    }
};

#endif // APPLICATION_H