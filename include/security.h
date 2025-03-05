#ifndef SECURITY_H
#define SECURITY_H

#include <sodium.h>
#include <string>
#include <vector>

class security_base 
{
private:
    unsigned char * assignKey(std::string stored_key_path, int type);

public:
    //Constructor to initialize Sodium and read the stored key
    explicit security_base();

    //Encrypts a plaintext string using the provided key
    std::vector<unsigned char> encrypt(const unsigned char* plaintext, int plaintext_len, unsigned char nonce[crypto_secretbox_NONCEBYTES], std::string board_id);

    //Decrypts a ciphertext
    std::vector<unsigned char> decrypt(const std::vector<unsigned char> ciphertext, int ciphertext_len, const unsigned char nonce[crypto_secretbox_NONCEBYTES], std::string board_id);

    //Generates and returns a random nonce of crypto_secretbox_NONCEBYTES bytes
    void generateNonce(unsigned char* nonce);
    
    //Generates a digital signature for the data
    void generateSignature(const unsigned char* msg, int msg_len, unsigned char* sig, std::string board_id);

    //Verifies the digital signature
    bool verifySignature(const unsigned char sig[crypto_sign_BYTES], const unsigned char* msg, int msg_len, std::string board_id);

    unsigned char * getKey(std::string board_id);
};

#endif // SECURITY_H
