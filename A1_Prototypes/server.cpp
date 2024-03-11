#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[1024];
    socklen_t addr_size;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    memset(&clientAddr, 0, sizeof(clientAddr));

    // Filling server information
    serverAddr.sin_family = AF_INET; // IPv4
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Any IP address
    serverAddr.sin_port = htons(PORT); // Port number

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) 
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("UDP server listening on port %d\n", PORT);

    while (1) 
    {
        addr_size = sizeof(clientAddr);
        int n = recvfrom(sockfd, (char *)buffer, 1024, 0, (struct sockaddr *)&clientAddr, &addr_size);
        buffer[n] = '\0';
        printf("Client: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
