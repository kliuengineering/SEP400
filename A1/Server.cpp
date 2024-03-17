#include <atomic>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <mutex>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <thread>

#define PORT 8080
#define SIZE_BUF 1024

// global vars
std::atomic<bool> is_running(true);
int fd_socket;
std::mutex mutex_log;
std::thread recv_thread;


// signal handler -> graceful shutdown
void ShutDownHandler(int sig)
{
    if(sig == SIGINT) is_running = false;
}


// receive thread
void ReceiveMessage(void)
{
    char buffer[SIZE_BUF];
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);

    while(is_running)
    {
        std::unique_lock<std::mutex> lock(mutex_log, std::defer_lock);
        int n = recvfrom(fd_socket, buffer, SIZE_BUF, 0, (struct sockaddr *)&client_addr, &len);

        if(n > 0)
        {
            lock.lock();

            std::cout << "Received message -> " << buffer << std::endl;
            // WIP: needs to implement a writing to a log file

            lock.unlock();
        }
        else
        {
            sleep(1);
        }
    }
}


// main()
int main(void)
{
    signal(SIGINT, ShutDownHandler);

    // non-blocking UDP socket
    fd_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd_socket < 0)
    {
        perror("Socket creation -> failed\n");
        exit(EXIT_FAILURE);
    }

    // non blocking sock
    int flag = fcntl(fd_socket, F_GETFL, 0);
    fcntl(fd_socket, F_SETFL, flag | O_NONBLOCK);

    // bind()
    struct sockaddr_in server_addr;
    memset(&server_addr, 9, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if ( bind(fd_socket, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0 )
    {
        perror("Bind -> failed.\n");
        close(fd_socket);
        exit(EXIT_FAILURE);
    }


    // starts the receive thread now
    int option = 5;

    do
    {
        std::cout << "1. Set log level\n";
        std::cout << "2. Dump log file\n";
        std::cout << "0. Shut down gracefully\n";
        std::cout << "Your option is... -> ";
        std::cin >> option; puts("");

        switch (option)
        {
            case 1:
                // WIP: implement set log level
            break;

            case 2:
                // WIP: implement dump log file
            break;
        }
    } 
    while (option && is_running);
    

    // shut down gracefully here
    if (recv_thread.joinable())
    {
        recv_thread.join();
    }
    close(fd_socket);
    return 0;
}