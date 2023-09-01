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

// Function to handle receiving messages from the server
void* receiveMessages(void* arg) {
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
            std::cout << "Server disconnected." << std::endl;
            break;
        } else {
            std::cout << "-> " << buffer << std::endl;
        }
    }

    // Close the client socket
    close(clientSocket);
    exit(EXIT_SUCCESS);
}

int main() {
    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Error creating client socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address structure
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);  // Port number
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Server IP address

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    std::cout << "Connected to server." << std::endl;

    // Start a thread to receive messages from the server
    pthread_t receiveThread;
    if (pthread_create(&receiveThread, NULL, receiveMessages, (void*)&clientSocket) != 0) {
        perror("Error creating receive thread");
        exit(EXIT_FAILURE);
    }

    // Send messages to the server
    char message[1024];
        std::cout << "Enter a message (or 'q' to quit) " << std::endl;
    while (true) {
        std::cin.getline(message, sizeof(message));

        if (strcmp(message, "q") == 0) {
            break;  // Quit the loop
        }

        ssize_t bytesSent = send(clientSocket, message, strlen(message), 0);
        if (bytesSent == -1) {
            perror("Error sending");
            break;
        }
    }

    // Close the client socket
    close(clientSocket);

    return 0;
}
