#pragma once
#include "Exception.hpp"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <string>


# define GREEN		"\033[0;32m"
# define RESET		"\033[0m"
#define BOT_PRIVMSG(nick, msg)		"PRIVMSG " + nick + " :" + msg + "\n"


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
        void                                createSocket();
        void                                serveraddrSocket();
        void                                clientSocket();
        void                                botRun();
        static int                          checkPortNumber(char *port);
        static std::vector<std::string>     splitString(const std::string& input);
        static std::vector<std::string>     splitFromComa(const std::string& input);
        void                                writeRpl(int fd, std::string msg);
};




