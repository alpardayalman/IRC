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
#include <vector>
#include <map>
#include <string>
#include "Client.hpp"
#include "Utilities.hpp"
#include "Chanel.hpp"

class Client;
class Server;

#define ERR_NONICKNAMEGIVEN     ":LAZYIRC 431 : No nickname given\r\n"

typedef std::vector<Chanel>::iterator    ChanelIterator;
typedef std::vector<Client>::iterator    ClientIterator;
typedef int (Server::*CmdFunct)(std::string& , Client&);
typedef std::map<std::string, CmdFunct>::iterator itCmdFunct;

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


    public:
        std::vector<Chanel>     chanels;

        void    initCommands( void );
        void    commandHandler(std::string, Client&);
        Server(size_t port_number, char * password);
        ~Server();

    private:
        void    createSocket( void );
        void    serveraddrSocket( void ) const;
        void    socketListen( void ) const;
        void    run( void );
        int     findMaxFd( void ) const;
        int     isClientIn(Chanel &cha, int cliFd);
        int     isClientIn(Client& cli, std::string nameChanel);

        // Commands ––
        std::map<std::string, CmdFunct> t_cmdFunct;
        int     Pass(std::string &s, Client& cli);
        int     checkPassword(std::string& s, Client& c);
        int     Info(std::string &s, Client& cli);
        int     PrivMsg(std::string &s, Client& cli);
        int     Join(std::string &s, Client &cli);
        int     Nick(std::string &s, Client &cli);
        int     User(std::string &s, Client &cli);
        int     Cap(std::string &s, Client &cli);
        int     Topic(std::string &s, Client &cli);
        int     Invite(std::string &s, Client &cli);
        int     Notice(std::string &s, Client &cli);
};
