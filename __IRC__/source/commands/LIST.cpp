#include "Server.hpp"

int    Server::List(std::string &s, Client& cli) {
    (void) s;
    Utilities::writeRpl(cli.cliFd, RPL_LISTSTART(cli.nick, Utilities::intToString(this->clients.size())));
    for (ChanelIterator it = this->chanels.begin(); it != this->chanels.end(); ++it) {
        if (!it->topic.length())
            Utilities::writeRpl(cli.cliFd, RPL_LIST(cli.nick, it->name, Utilities::intToString(it->clients.size()), " Empty"));
        else
            Utilities::writeRpl(cli.cliFd, RPL_LIST(cli.nick, it->name, Utilities::intToString(it->clients.size()), it->topic));
    }
    Utilities::writeRpl(cli.cliFd, RPL_LISTEND(cli.nick));
    return 1;
}
