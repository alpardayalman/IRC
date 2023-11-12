#include "Bot.hpp"

Bot::Bot(int port_number, char *password): port_number(port_number), password(std::string(password)), username("Bot"), nickname("Bot") {
    this->clientSocket();
    this->botRun();
}
Bot::~Bot() {
    close(this->sockFd);
}

void Bot::botRun() {

    int login = 1;
    while (true) {
        fd_set read_set;
        FD_ZERO(&read_set);
        FD_SET(this->sockFd, &read_set);

        timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        // Wait for data or timeout
        int activity = select(this->sockFd + 3, &read_set, nullptr, nullptr, &timeout);

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
            std::string umsg = buffer;
            std::vector<std::string> cmd = splitString(umsg);
            std::cout << umsg << std::endl;
            if (cmd[0] == "NAMES") {
                std::cout << "Names are comming" << std::endl;
            }
            // Your welcome message logic here
            std::string msg;
            if (login) {
                msg = "CAP BOT\nPASS "+this->password+"\n"+ "NICK "+this->nickname +"\n"+ "USER "+this->username + "\n";
            }
            if (login) {
                write(this->sockFd, msg.c_str(), msg.length());
                login = 0;
            }
            else if (cmd[0] == "NAMES") {
                for (int i = 1; i < (int) cmd.size() ; i++) {
                    writeRpl(this->sockFd, BOT_PRIVMSG(cmd[i], "Welcome"));
                }
            }
            else if (cmd[0] == "NAMESS") {
                for (int i = 2; i < (int) cmd.size(); i++) {
                    writeRpl(this->sockFd, BOT_PRIVMSG(cmd[i], cmd[1]));
                }
            }
        }
    }
    close(this->sockFd);
}