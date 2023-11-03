#include "../include/Server.hpp"


int     Server::Info(std::string &s, Client& cli) {
    std::cout << "INFO command is called." << std::endl;
    if (!cli.passcheku)
        return 0;
    std::cout << "NICK:" << cli.nick << " USER:" << cli.user << s << std::endl;
    return 1;
    
}
