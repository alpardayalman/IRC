#include "Server.hpp"

int     Server::Notice(std::string &s, Client &cli) {
    std::vector<std::string> param = Utilities::tokenCmd(s, 0);
    for (ClientIterator it = clients.begin(); it != clients.end(); ++it) {
        if (it->nick == param[0] || it->user == param[0]) {
            (*it).messageBox.push_back(RPL_NOTICE(cli.getPrefix(), it->nick, param[1]));
            FD_SET((*it).cliFd, &this->writeFds);
        }
        else if(isClientIn((*it), param[0]) && cli.nick != it->nick) {
            (*it).messageBox.push_back(RPL_NOTICE(cli.getPrefix(), param[0], param[1]));
            FD_SET((*it).cliFd, &this->writeFds);
        }
    }
    return 0;
}