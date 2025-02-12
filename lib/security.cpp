#include "security.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <iomanip>

using namespace std;

//Initialize Sodium
security_base::security_base(string stored_key_path) 
{
    sodium_init();

    //Read the stored key	
    string key_string;

    ifstream key_file(stored_key_path);
    
    getline(key_file, key_string);
    
    for(int i = 0; i < crypto_secretbox_KEYBYTES; i++)
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
    //TODO: Uncomment the randomized nonce
    //randombytes_buf(nonce, sizeof nonce);
    string nonce_string;
    ifstream nonce_file("nonce_boards.txt");
    
    getline(nonce_file, nonce_string);

    for(int i = 0; i < crypto_secretbox_NONCEBYTES; i++)
    {
        nonce[i] = stoi(nonce_string.substr(i*2, 2), nullptr, 16);
    }
}


