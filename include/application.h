#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include "udp_sub.h"
#include "packet.pb.h"
#include "security.h"
#include <vector>

using namespace std;
security_base symmetric_key_security_agent("../symmetric_key_boards.txt", 0);

security_base signature_verifier_security_agent("../public_key_boards.txt", 2);

class Application
{
public:
    Application(bool debug, bool debug_sub);
    void update();

    bool get_proto_packet(std::string packet_str, capstone_protobuf::Packet &packet_output);

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

    bool verifyDigitalSignature(std::string data, std::string signature)
    {

        unsigned char sig[crypto_sign_BYTES];

        //Conver the signature string into an array of unsigned characters
        copy(signature.begin(), signature.end(), sig);

        bool verified = signature_verifier_security_agent.verifySignature(sig, (unsigned char*)data.data(), data.length());

        return verified;
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