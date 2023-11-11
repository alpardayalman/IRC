#pragma once

#include <iostream>
#include <vector>
#include "Utilities.hpp"
#include "Server.hpp"

#define HEX 1
#define NC  2

class Client
{
    public:
        int     isHex;
        std::vector<std::string> messageBox;
        int cliFd;
        int port;
        char ipAddr[INET_ADDRSTRLEN];
        std::string ip;
        std::string user;
        std::string nick;
        std::string pass;

        int         isCapt;
        int         passcheku;
        std::string buffer;

        Client() : isHex(0), isCapt(0), passcheku(0), buffer("")  {}
        ~Client() {}
        const Client &operator=(const Client &copy);

        std::string	        getPrefix() const;
};