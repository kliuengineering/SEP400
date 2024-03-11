#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080

int main() 
{
    int sockfd;
    struct sockaddr_in serverAddr;
    char *message = "Hello from client";
    char buffer[1024];

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));

    // Server information
    serverAddr.sin_family = AF_INET; // IPv4
    serverAddr.sin_port = htons(PORT); // Port number
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Any IP address

    int n, len;

    sendto(sockfd, (const char *)message, strlen(message), 0, (const struct sockaddr *) &serverAddr, sizeof(serverAddr));
    printf("Message sent.\n");

    close(sockfd);
    return 0;
}
