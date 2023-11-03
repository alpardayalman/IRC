#pragma once
#include <iostream>
#include <sstream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "Client.hpp"
#include "Chanel.hpp"
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
        fd_set                  readFds;
        fd_set                  writeFds;
        fd_set                  readFdsSup;
        fd_set                  writeFdsSup;
        char                    buffer[1024];
        std::vector<Chanel>     chanels;

    public:
        Server(size_t port_number, char * password);
        ~Server();
    
    private:
        void    createSocket( void );
        void    serveraddrSocket( void ) const;
        void    socketListen( void ) const;
        void    run( void );
        int     findMaxFd( void ) const;



        // Commands ––
        int     Pass(std::string &s, Client& cli);
        bool    checkPassword(std::string& s, Client& c);
        int     Info(std::string &s, Client& cli);
        int     PrivMsg(std::string &s, Client& cli);

};
