#include "security.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <iomanip>

using namespace std;

//Initialize Sodium
security_base::security_base() 
{
    sodium_init();	
}    

unsigned char * security_base::assignKey(std::string stored_key_path, int type)
{
    //The key to be returned
    unsigned char * key;

    //Assign the value of the key
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

    //Read the stored key	
    string key_string;

    ifstream key_file(stored_key_path);
    

    if (!key_file.is_open())
    {
        throw runtime_error("Failed to open key file");
        key = '\0';
        return key;
    }

    getline(key_file, key_string);
    
    //Convert the key string to a byte array
    for(int i = 0; i < key_size; i++)
    {
        key[i] = stoi(key_string.substr(i*2, 2), nullptr, 16);
    }
}

// Encrypts a plaintext string using the provided key.
vector<unsigned char> security_base::encrypt(const unsigned char * plaintext, int plaintext_len, unsigned char nonce[crypto_secretbox_NONCEBYTES], int board_id) 
{
    int ct_len = plaintext_len + crypto_secretbox_MACBYTES;
    
    //Get the private of the specific board
    string key_path = string(getenv("HOME")) + "/.capstone_keys/symmetric_key_" + to_string(board_id) + ".txt";

    unsigned char * key = assignKey(key_path, 0);

    // Resize the ciphertext buffer to hold MAC bytes plus the plaintext.
    vector<unsigned char> ciphertext(ct_len);

    // Encrypt using crypto_secretbox_easy.
    crypto_secretbox_easy(ciphertext.data(), plaintext, plaintext_len, nonce, key);
    
    return ciphertext;
}
    
// Decrypts a ciphertext
// Returns the decrypted plaintext string.
vector<unsigned char> security_base::decrypt(vector<unsigned char> ciphertext, int ciphertext_len, const unsigned char nonce[crypto_secretbox_NONCEBYTES], int board_id) 
{
    size_t pt_len = ciphertext_len - crypto_secretbox_MACBYTES;

    vector<unsigned char> plaintext(pt_len + 1);

    //Get the private of the specific board
    string key_path = string(getenv("HOME")) + "/.capstone_keys/symmetric_key_" + to_string(board_id) + ".txt";

    unsigned char * key = assignKey(key_path, 0);

    // Decrypt using crypto_secretbox_open_easy.
    if (crypto_secretbox_open_easy(plaintext.data(), ciphertext.data(), ciphertext_len, nonce, key) != 0) 
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
void security_base::generateSignature(const unsigned char* msg, int msg_len, unsigned char* sig, int board_id)
{
    //Get the private of the specific board
    string key_path = string(getenv("HOME")) + "/.capstone_keys/private_key_" + to_string(board_id) + ".txt";
    unsigned char * key = assignKey(key_path, 1);

    //The key in this function will be the private key
    crypto_sign_detached(sig, NULL, msg, msg_len, key);
}

//Verifies the digital signature
bool security_base::verifySignature(const unsigned char sig[crypto_sign_BYTES], const unsigned char* msg, int msg_len, int board_id)
{
    //Get the public key of the specific board
    string key_path = string(getenv("HOME")) + "/.capstone_keys/public_key_" + to_string(board_id) + ".txt";
    unsigned char * key = assignKey(key_path, 2);

    //The key in this function will be the public key
    if(crypto_sign_verify_detached(sig, msg, msg_len, key) != 0) 
    {
        return false;
    }
    else
    {
        return true;
    }
}

unsigned char * security_base::getKey(int board_id)
{
    //Get the public key of the specific board
    string key_path = string(getenv("HOME")) + "/.capstone_keys/public_key_" + to_string(board_id) + ".txt";
    unsigned char * key = assignKey(key_path, 2);
    
    return key;
}


