#include "Server.hpp"

int    Server::List(std::string &s, Client& cli) {
    (void) s;
    Utilities::writeRpl(cli.cliFd, RPL_LISTSTART(cli.nick, static_cast<char>(this->clients.size())));
    for (ChanelIterator it = this->chanels.begin(); it != this->chanels.end(); ++it) {
        if (!it->topic.length())
            Utilities::writeRpl(cli.cliFd, RPL_LIST(cli.nick, it->name, static_cast<char>(it->clients.size()), " Empty"));
        else
            Utilities::writeRpl(cli.cliFd, RPL_LIST(cli.nick, it->name, static_cast<char>(it->clients.size()), it->topic));
    }
    Utilities::writeRpl(cli.cliFd, RPL_LISTEND(cli.nick));
    return 1;
}