#include "Server.hpp"


int    Server::List(std::string &s, Client& cli) {
    (void) s;
    Utilities::writeRpl(cli.cliFd, RPL_LISTSTART(cli.nick, std::to_string(this->clients.size())));
    for (ChanelIterator it = this->chanels.begin(); it != this->chanels.end(); ++it) {
        if (!it->topic.length())
            Utilities::writeRpl(cli.cliFd, RPL_LIST(cli.nick, it->name, std::to_string(it->clients.size()), " Empty"));
        else
            Utilities::writeRpl(cli.cliFd, RPL_LIST(cli.nick, it->name, std::to_string(it->clients.size()), it->topic));

    }
    Utilities::writeRpl(cli.cliFd, RPL_LISTEND(cli.nick));
    return 1;
}