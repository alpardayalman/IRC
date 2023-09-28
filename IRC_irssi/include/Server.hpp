#pragma once
#include <iostream>

class Server {
    private:
        size_t port_number;
        const std::string password;
    public:
        Server(size_t port_number, char * password);
        ~Server();
};