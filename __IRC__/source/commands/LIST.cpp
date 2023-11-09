#include "Server.hpp"


int    Server::List(std::string &s, Client& cli) {
    (void) s;
    std::string msg = RPL_LISTSTART(cli.nick);
    write(cli.cliFd, msg.c_str(), msg.length());
    for (ChanelIterator it = this->chanels.begin(); it != this->chanels.end(); ++it) {
        msg = RPL_LIST(cli.nick, it->name, std::to_string(it->clients.size()));
        write(cli.cliFd, msg.c_str(), msg.length());
    }
    msg = RPL_LISTEND(cli.nick);
    write(cli.cliFd, msg.c_str(), msg.length());
    return 1;
}