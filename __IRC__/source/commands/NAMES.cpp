#include "Server.hpp"
#include "Exception.hpp"

int     Server::Names(std::string &s, Client &cli) {
    (void) cli;
    std::cout << s << std::endl;
    ClientIterator it = this->clients.begin();
    for (; it != this->clients.end(); ++it) {
        std::cout << it->nick << " " << it->isCapt << std::endl;
        if (it->isCapt == BOT) {
            break;
        }
    }
    if (it == this->clients.end()) {
        return 0;
    }

    std::string msg = "NAMES";
    if (s != "")
        msg += "S " + s;
    for (ClientIterator cit = this->clients.begin(); cit != this->clients.end(); ++cit) {
        if (cit->isCapt == BOT)
            continue;
        msg+= " " + cit->nick;
    }
    write(it->cliFd, msg.c_str(), msg.length());
    return 0;
}
