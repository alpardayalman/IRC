#include "Server.hpp"
#include "Exception.hpp"

// RPL_PRIVMSG(cli.getPrefix(), (*it).nick, params[1])
int     Server::Names(std::string &s, Client &cli) {
    (void) cli;
    std::cout << s << std::endl;
    ClientIterator it = this->clients.begin();
    for (; it != this->clients.end(); ++it) {
        std::cout << it->nick << " " << it->isCapt << std::endl;
        if (it->isCapt == BOT) {
            std::cout << "BOTLANDIN" << std::endl;
            break;
        }
    }
    if (it == this->clients.end()) {
        return 0;
    }

    std::cout << "names a" << std::endl;
    std::string msg = "NAMES";
    if (s != "")
        msg += "S " + s + " ";
    for (ClientIterator cit = this->clients.begin(); cit != this->clients.end(); ++cit) {
        if (cit->isCapt == BOT)
            continue;
        msg+= cit->nick + " "; // bakmaya devam et.
    }
    (write(it->cliFd, msg.c_str(), msg.length()) < 0) ?
    std::cout << "Write Mistake" << std::endl:
        0;
    return 0;
}
