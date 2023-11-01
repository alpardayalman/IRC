#pragma once
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "Client.hpp"
#include <vector>

class Server {
    private:
        size_t                  port_number;
        const std::string       password;
        size_t                  server_fd;
        bool                    is_running;
        int                     reuse;
        const std::string       server_name;
        std::vector<Client>     clients; // client vectoru.

    public:
        Server(size_t port_number, char * password);
        ~Server();
    
    private:
        void    createSocket( void );
        void    serveraddrSocket( void ) const;
        void    socketListen( void ) const;
        void    run( void );
        int     findMaxFd( void ) const;
        bool    AddUser_checkPassword(std::string& s, Client& c);
};