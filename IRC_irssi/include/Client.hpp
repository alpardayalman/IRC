#pragma once
#include <iostream>
#include <vector>
class Client
{
    public:
        std::vector<std::string> messageBox;
        int cliFd;
        int port;
        std::string ipAddr;
        std::string user;
        std::string nick;
        std::string pass;


        int         passcheku;
        Client() : passcheku(0) {}
        ~Client() {}
};