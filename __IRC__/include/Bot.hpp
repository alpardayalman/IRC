#pragma once
#include "Exception.hpp"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>


# define GREEN		"\033[0;32m"
# define RESET		"\033[0m"

class Bot {
    private:
        int         port_number;
        std::string password;
        std::string username;
        std::string nickname;
        int         sockFd;
        int                 reuse;
        struct sockaddr_in  addr_socket;
    public:
        Bot(int, char *);
        ~Bot();
        void                        createSocket();
        void                        serveraddrSocket();
        void                        socketConnect();
        void                        botRun();
        static int                         checkPortNumber(char *port);
        static std::vector<std::string>    splitString(const std::string& input);

};




