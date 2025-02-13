#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define BROADCAST_IP "255.255.255.255"
#define PORT 12345
#define MESSAGE "Hello, broadcast!"

int main()
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("Socket creation failed");
        return 1;
    }

    int broadcastEnable = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) < 0)
    {
        perror("Failed to enable broadcast");
        close(sock);
        return 1;
    }

    struct sockaddr_in broadcastAddr{};
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_port = htons(PORT);
    broadcastAddr.sin_addr.s_addr = inet_addr(BROADCAST_IP);

    ssize_t sentBytes = sendto(sock, MESSAGE, strlen(MESSAGE), 0,
                               (struct sockaddr *)&broadcastAddr, sizeof(broadcastAddr));
    if (sentBytes < 0)
    {
        perror("Broadcast failed");
    }
    else
    {
        std::cout << "Broadcast message sent: " << MESSAGE << std::endl;
    }

    close(sock);
    return 0;
}
