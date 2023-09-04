#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>
#include <vector>

std::vector<int> clientSockets;  // Store client sockets

// Function to broadcast a message to all connected clients
void broadcastMessage(const char* message, int senderSocket) {
    for(std::vector<int>::iterator it = clientSockets.begin(); it != clientSockets.end(); ++it)
    {
        int socket = static_cast<int>((*it));
        if (socket != senderSocket) {
            send(socket, message, strlen(message), 0);
        }
    }
}

// Function to handle client communication in a separate thread
void* handleClient(void* arg) {
    int clientSocket = *((int*)arg);
    char buffer[1024];
    ssize_t bytesRead;

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead == -1) {
            perror("Error receiving");
            break;
        } else if (bytesRead == 0) {
            std::cout << "Client disconnected." << std::endl;
            break;
        } else {
            std::cout << "Received from client: " << buffer << std::endl;

            // Broadcast the received message to all clients
            broadcastMessage(buffer, clientSocket);
        }
    }

    // Close the client socket and remove it from the list
    close(clientSocket);
    clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), clientSocket), clientSockets.end());

    return NULL;
}

int main() {
    // Create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Error creating server socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address structure
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);  // Port number
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // Bind to any available network interface

    // Bind the socket to the address and port
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error binding");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) == -1) {
        perror("Error listening");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is listening on port 8080..." << std::endl;

    // Accept and handle incoming connections in separate threads
    while (true) {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int* clientSocket = new int;

        *clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (*clientSocket == -1) {
            perror("Error accepting connection");
            delete clientSocket;
            continue;
        }

        std::cout << "Client connected from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;

        // Add the client socket to the list
        clientSockets.push_back(*clientSocket);

        pthread_t clientThread;
        if (pthread_create(&clientThread, NULL, handleClient, (void*)clientSocket) != 0) {
            perror("Error creating thread");
            delete clientSocket;
            continue;
        }

        pthread_detach(clientThread);
    }

    // Close the server socket (unreachable in this example)
    close(serverSocket);

    return 0;
}
