#include "Server.hpp"

int     Server::Notice(std::string &s, Client &cli) {
    std::vector<std::string> param = Utilities::tokenCmd(s);
    for (ClientIterator it = clients.begin(); it != clients.end(); ++it) {
        if (it->nick == param[0] || it->user == param[0]) {
            // std::cout << "notice,nick:"<< it->nick << std::endl;
            std::string a = RPL_NOTICE(cli.getPrefix(), it->nick, param[1].substr(1, param[1].length()));
            // Utilities::fd_write_color(1, a.c_str(), PURPLE);
            write(it->cliFd, a.c_str(), a.length());
            // it->messageBox.push_back(a);
        }
    }
    return 0;
}