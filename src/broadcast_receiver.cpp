#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in recvAddr{};
    recvAddr.sin_family = AF_INET;
    recvAddr.sin_port = htons(PORT);
    recvAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr *)&recvAddr, sizeof(recvAddr)) < 0)
    {
        perror("Bind failed");
        close(sock);
        return 1;
    }

    std::cout << "Listening for broadcast messages on port " << PORT << "..." << std::endl;

    char buffer[BUFFER_SIZE];
    struct sockaddr_in senderAddr{};
    socklen_t senderAddrLen = sizeof(senderAddr);

    while (true)
    {
        ssize_t receivedBytes = recvfrom(sock, buffer, BUFFER_SIZE - 1, 0,
                                         (struct sockaddr *)&senderAddr, &senderAddrLen);
        if (receivedBytes > 0)
        {
            buffer[receivedBytes] = '\0';
            std::cout << "Received broadcast from "
                      << inet_ntoa(senderAddr.sin_addr) << ": " << buffer << std::endl;
        }
    }

    close(sock);
    return 0;
}
