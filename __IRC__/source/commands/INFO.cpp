#include "Server.hpp"


int     Server::Info(std::string &s, Client& cli) {
    std::cout << "INFO command is called." << std::endl;
    if (!cli.passcheku)
        return 0;
    // std::cout << "NICK:" << cli.nick << " USER:" << cli.user << s << std::endl;
    for (ChanelIterator it = this->chanels.begin(); it != this->chanels.end(); ++it) {
        if (it->name == Utilities::trim(s))
            whoIsInChanel(*it);
    }
    return 1;
    
}
