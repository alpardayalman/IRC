#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 55555
#define MAX_CONNECTIONS 5
#define MAX_BUFFER 1024

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    char buffer[MAX_BUFFER];

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
        // Accept connection
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket == -1) {
            perror("Error accepting connection");
            close(serverSocket);
            return 1;
        }

        std::cout << "Accepted connection from " << inet_ntoa(clientAddr.sin_addr) << std::endl;

        while (true) {
            // Receive data from client
            int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (bytesRead <= 0) {
                break;
            }

            buffer[bytesRead] = '\0';
            std::cout << "Received from client: " << buffer << std::endl;

            // Process received data if needed
            // ...

            // Send response back to client
            send(clientSocket, buffer, strlen(buffer), 0);
        }

        close(clientSocket);
        std::cout << "Connection closed" << std::endl;
    }

    close(serverSocket);

    return 0;
}
