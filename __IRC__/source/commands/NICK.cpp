#include "Server.hpp"

int     Server::Nick(std::string &s, Client &cli) {
    std::stringstream ss(s);
    ss >> s;

    if (!s.empty()) {
        cli.messageBox.push_back(RPL_NICK(cli.nick, cli.user, cli.ip, s));
        FD_SET(cli.cliFd, &this->writeFds);
        for (ChanelIterator it = this->chanels.begin(); it != this->chanels.end(); ++it) {
            for (ClientIterator cit = (*it).clients.begin(); cit != (*it).clients.end(); ++cit) {
                if (cli.nick == cit->nick)
                {
                    cit->nick = s;
                    break;
                }
            }
        }
        cli.nick = s;
    }
    return 0;
}
