#include "Server.hpp"

int Server::Invite(std::string &s, Client &cli) {

    std::vector<std::string> cmd = Utilities::tokenCmd(s, 1);
    for (std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end(); ++it) {
        if ((*it).nick == cmd[0]) {
            (*it).messageBox.push_back(RPL_INVITE(cli.nick, (*it).ipAddr, cmd[1], cmd[0]));
            FD_SET((*it).cliFd, &this->writeFds);
        }
    }
    return 1;
}
