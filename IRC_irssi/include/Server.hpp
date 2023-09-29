#pragma once
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

class Server {
    private:
        size_t                  port_number;
        const std::string       password;
        size_t                  server_fd;
        bool                    is_running;
        int                     reuse;
        const std::string       server_name;
    public:
        Server(size_t port_number, char * password);
        ~Server();
    
    private:
        void    createSocket( void );
        void    serveraddrSocket( void ) const;
        void    socketListen( void ) const;


};