#include "Bot.hpp"

Bot::Bot(int port_number, char *password): port_number(port_number), password(std::string(password)) {
    this->createSocket();
    this->serveraddrSocket();
    this->socketConnect();
    this->botRun();
}
Bot::~Bot() {close(this->sockFd);}

void Bot::botRun() {

    while (true) {
        fd_set read_set;
        FD_ZERO(&read_set);
        FD_SET(this->sockFd, &read_set);

        timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        // Wait for data or timeout
        int activity = select(this->sockFd + 1, &read_set, nullptr, nullptr, &timeout);

        if (activity == -1) {
            perror("Select error");
            break;
        } else if (activity == 0) {
            // Timeout, do something if needed
            continue;
        } else {
            // Data is ready to be read
            char buffer[1024];
            int bytes_received = recv(this->sockFd, buffer, sizeof(buffer), 0);

            if (bytes_received <= 0) {
                // Connection closed or error
                std::cerr << "Connection closed." << std::endl;
                break;
            }

            // Process the received data (print it for simplicity)
            buffer[bytes_received] = '\0';
            std::cout << "Received: " << buffer << std::endl;

            // Your welcome message logic here
            const char* msg = "Welcome to the server!\r\n";
            send(this->sockFd, msg, strlen(msg), 0);
        }
    }
}