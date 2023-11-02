#include "../include/Server.hpp"


int     Server::Info(std::string &s, Client& cli) {
    if (!cli.passcheku)
        return 0;
    std::cout << "NICK:" << cli.nick << " USER:" << cli.user << s << std::endl;
    return 1;
    
}
