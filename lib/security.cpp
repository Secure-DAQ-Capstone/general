#include "security.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <iomanip>

using namespace std;

//Initialize Sodium
security_base::security_base(string stored_key_path, int type) 
{
    sodium_init();

    //Read the stored key	
    string key_string;

    ifstream key_file(stored_key_path);
    
    getline(key_file, key_string);
    
    int key_size;

    switch(type)
    {
        case 0:
            key_size = crypto_secretbox_KEYBYTES;
            break;
        case 1:
            key_size = crypto_sign_SECRETKEYBYTES;
            break;
        case 2:
            key_size = crypto_sign_PUBLICKEYBYTES;
            break;
        default:
            key_size = crypto_secretbox_KEYBYTES;
    }

    key = new unsigned char[key_size];
    
    for(int i = 0; i < key_size; i++)
    {
        this->key[i] = stoi(key_string.substr(i*2, 2), nullptr, 16);
    }	
}    
// Encrypts a plaintext string using the provided key.
vector<unsigned char> security_base::encrypt(const unsigned char * plaintext, int plaintext_len, unsigned char nonce[crypto_secretbox_NONCEBYTES]) 
{
    int ct_len = plaintext_len + crypto_secretbox_MACBYTES;

    // Resize the ciphertext buffer to hold MAC bytes plus the plaintext.
    vector<unsigned char> ciphertext(ct_len);

    // Encrypt using crypto_secretbox_easy.
    crypto_secretbox_easy(ciphertext.data(), plaintext, plaintext_len, nonce, this->key);
    
    return ciphertext;
}
    
// Decrypts a ciphertext
// Returns the decrypted plaintext string.
vector<unsigned char> security_base::decrypt(vector<unsigned char> ciphertext, int ciphertext_len, const unsigned char nonce[crypto_secretbox_NONCEBYTES]) 
{
    size_t pt_len = ciphertext_len - crypto_secretbox_MACBYTES;

    vector<unsigned char> plaintext(pt_len + 1);

    // Decrypt using crypto_secretbox_open_easy.
    if (crypto_secretbox_open_easy(plaintext.data(), ciphertext.data(), ciphertext_len, nonce, this->key) != 0) 
    {
        throw runtime_error("Decryption failed");
    }
    
    plaintext[pt_len] = '\0';
    return plaintext;
}

    
// Generates and returns a random nonce of crypto_secretbox_NONCEBYTES bytes.
void security_base::generateNonce(unsigned char * nonce) 
{
    randombytes_buf(nonce, sizeof nonce);
}

//Generates a digital signature for the data
void security_base::generateSignature(const unsigned char* msg, int msg_len, unsigned char* sig)
{
    //The key in this function will be the private key
    crypto_sign_detached(sig, NULL, msg, msg_len, this->key);
}

//Verifies the digital signature
bool security_base::verifySignature(const unsigned char sig[crypto_sign_BYTES], const unsigned char* msg, int msg_len)
{
    //The key in this function will be the public key
    if(crypto_sign_verify_detached(sig, msg, msg_len, this->key) != 0) 
    {
        return false;
    }
    else
    {
        return true;
    }
}

std::string security_base::getKey()
{
    string key_str;

    //Display the key as a hexadecimal string
    std::ostringstream oss;
    for (size_t i = 0; i < crypto_sign_PUBLICKEYBYTES; i++) 
    {
        oss << hex << setw(2) << setfill('0') << (int)this->key[i];
    }
    key_str = oss.str();
    
    return key_str;
}


