#pragma once

#include <iostream>
#include <vector>
#include "Utilities.hpp"
#include "Server.hpp"

class Client
{
    public:
        int     isHex;
        std::vector<std::string> messageBox;
        int cliFd;
        int port;
        char ipAddr[INET_ADDRSTRLEN];
        std::string user;
        std::string nick;
        std::string pass;


        int         passcheku;
        Client() : isHex(0), passcheku(0)  {}
        ~Client() {}

        void    colorWrite(std::string &msg, const char *color);
};