#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

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

    // Send and receive messages
    char message[1024];
    while (true) {
        std::cout << "Enter a message (or 'q' to quit): ";
        std::cin.getline(message, sizeof(message));

        if (strcmp(message, "q") == 0) {
            break;  // Quit the loop
        }

        ssize_t bytesSent = send(clientSocket, message, strlen(message), 0);
        if (bytesSent == -1) {
            perror("Error sending");
            break;
        }

        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead == -1) {
            perror("Error receiving");
            break;
        } else if (bytesRead == 0) {
            std::cout << "Server disconnected." << std::endl;
            break;
        } else {
            std::cout << "Received: " << buffer << std::endl;
        }
    }

    // Close the socket
    close(clientSocket);

    return 0;
}
