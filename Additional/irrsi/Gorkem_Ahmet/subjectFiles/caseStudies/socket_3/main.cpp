#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <sys/stat.h>

#define PORT 8080
#define MAX_CONNECTIONS 5
#define MAX_BUFFER 1024

void handleClient(int clientSocket);

int main() {
    int serverSocket, clientSockets[MAX_CONNECTIONS], maxClients = 0;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);

    for (int i = 0; i < MAX_CONNECTIONS; ++i) {
        clientSockets[i] = -1;
    }

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Error creating socket");
        return 1;
    }

    // Bind
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error binding");
        close(serverSocket);
        return 1;
    }

    // Listen
    if (listen(serverSocket, MAX_CONNECTIONS) == -1) {
        perror("Error listening");
        close(serverSocket);
        return 1;
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    while (true) {
        struct pollfd fds[MAX_CONNECTIONS + 1];
        memset(fds, 0, sizeof(fds));
        fds[0].fd = serverSocket;
        fds[0].events = POLLIN;

        int currentClients = 1;
        for (int i = 0; i < MAX_CONNECTIONS; ++i) {
            int socket = clientSockets[i];
            if (socket != -1) {
                fds[currentClients].fd = socket;
                fds[currentClients].events = POLLIN;
                ++currentClients;
            }
        }

        int activity = poll(fds, currentClients, -1);

        if (activity < 0) {
            perror("Error in poll");
            close(serverSocket);
            return 1;
        }

        if (fds[0].revents & POLLIN) {
            int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
            if (clientSocket == -1) {
                perror("Error accepting connection");
                close(serverSocket);
                return 1;
            }

            if (maxClients < MAX_CONNECTIONS) {
                for (int i = 0; i < MAX_CONNECTIONS; ++i) {
                    if (clientSockets[i] == -1) {
                        clientSockets[i] = clientSocket;
                        ++maxClients;
                        break;
                    }
                }
            } else {
                std::cout << "Too many clients. Connection refused." << std::endl;
                close(clientSocket);
            }

            std::cout << "Accepted connection from " << inet_ntoa(clientAddr.sin_addr) << std::endl;
        }

        for (int i = 1; i < currentClients; ++i) {
            int socket = fds[i].fd;
            if (socket != -1 && fds[i].revents & POLLIN) {
                handleClient(socket);
            }
        }
    }

    close(serverSocket);

    return 0;
}

void handleClient(int clientSocket) {
    char buffer[MAX_BUFFER];
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytesRead <= 0) {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrSize = sizeof(clientAddr);
        getpeername(clientSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
        std::cout << "Connection closed by client " << inet_ntoa(clientAddr.sin_addr) << std::endl;
        close(clientSocket);
        return;
    }

    buffer[bytesRead] = '\0';
    std::cout << "Received from client: " << buffer << std::endl;
    send(clientSocket, buffer, strlen(buffer), 0);

    // Handle client message here
}
