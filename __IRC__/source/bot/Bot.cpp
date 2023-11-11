#include "Bot.hpp"

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>

int main() {
    const char* server_ip = "127.0.0.1";
    const int server_port = 8080;

    // Create socket
    int irc_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (irc_socket == -1) {
        perror("Socket creation failed");
        return -1;
    }

    // Set up server information
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip, &server_address.sin_addr);

    // Connect to the server
    if (connect(irc_socket, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address)) == -1) {
        perror("Connection failed");
        close(irc_socket);
        return -1;
    }

    while (true) {
        fd_set read_set;
        FD_ZERO(&read_set);
        FD_SET(irc_socket, &read_set);

        timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        // Wait for data or timeout
        int activity = select(irc_socket + 1, &read_set, nullptr, nullptr, &timeout);

        if (activity == -1) {
            perror("Select error");
            break;
        } else if (activity == 0) {
            // Timeout, do something if needed
            continue;
        } else {
            // Data is ready to be read
            char buffer[1024];
            int bytes_received = recv(irc_socket, buffer, sizeof(buffer), 0);

            if (bytes_received <= 0) {
                // Connection closed or error
                std::cerr << "Connection closed." << std::endl;
                break;
            }

            // Process the received data (print it for simplicity)
            buffer[bytes_received] = '\0';
            std::cout << "Received: " << buffer << std::endl;

            // Your welcome message logic here
            const char* welcome_message = "Welcome to the server!\r\n";
            send(irc_socket, welcome_message, strlen(welcome_message), 0);
        }
    }

    // Close the socket
    close(irc_socket);

    return 0;
}