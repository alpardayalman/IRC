#include "Server.hpp"


int     Server::Info(std::string &s, Client& cli) {
    if (!cli.passcheku)
        return 0;
    std::cout << "Nick: " << cli.nick << " User: " << cli.user << std::endl;
    for (ChanelIterator it = this->chanels.begin(); it != this->chanels.end(); ++it) {
        if (it->name == Utilities::trim(s))
            whoIsInChanel(*it);
    }
    return 1;
    
}
