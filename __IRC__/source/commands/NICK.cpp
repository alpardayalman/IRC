#include "Server.hpp"

// #define DEBUG_NICK
// server ->cli ->chanel
// server -> chanel ->cli
int     Server::Nick(std::string &s, Client &cli) {
    std::stringstream ss(s);
    ss >> s;
    if (!s.empty()) {
        std::string rpl = RPL_NICK(cli.nick, cli.user, cli.ip, s);
        write(cli.cliFd, rpl.c_str(), rpl.length());
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
