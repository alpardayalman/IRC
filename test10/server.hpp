#pragma once

#include "unistd.h"
#include "iostream"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "sys/types.h"
#include "string.h"
#include "vector"
#include "sys/select.h"


int createSocket(int portNo);

class client
{
    public:
        std::vector<std::string> messageBox;
        int cliFd;
        int port;
        std::string ipAddr;
};


class server
{
    public:
        std::vector<client> clients;
        int serverFd;
        int findMaxFd();
};