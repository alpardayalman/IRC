#include "Bot.hpp"




void Bot::clientSocket() {
    // Create socket
    this->sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->sockFd < 0) {
        perror("Socket creation failed");
        throw Exception("Socket creation failed.");
    }

    // Set up server address
    memset(&this->addr_socket, 0, sizeof(struct sockaddr_in));
    this->addr_socket.sin_family = AF_INET;
    this->addr_socket.sin_port = htons(this->port_number);
    inet_pton(AF_INET, "127.0.0.1", &this->addr_socket.sin_addr);

    // Connect to the server
    if (connect(this->sockFd, (struct sockaddr*)&this->addr_socket, sizeof(this->addr_socket)) < 0) {
        perror("Connect failed");
        throw Exception("Connection failed.");
    }

    std::cout << GREEN << "Success: Connected to the server." << RESET << std::endl;
}