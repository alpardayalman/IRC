#include "Server.hpp"

int     Server::Notice(std::string &s, Client &cli) {

    std::vector<std::string> param = Utilities::tokenCmd(s, 0);

    for (ClientIterator it = clients.begin(); it != clients.end(); ++it) {

        if (it->nick == param[0] || it->user == param[0]) {
            std::string a = RPL_NOTICE(cli.getPrefix(), it->nick, param[1].substr(1, param[1].length()));
            write(it->cliFd, a.c_str(), a.length());
        }
    }
    return 0;
}