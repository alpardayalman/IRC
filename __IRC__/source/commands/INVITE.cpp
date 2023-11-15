#include "Server.hpp"

int Server::Invite(std::string &s, Client &cli) {

    std::vector<std::string> cmd = Utilities::tokenCmd(s, 1);
    Chanel cha = getChanel(cmd[1]);
    for (std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end(); ++it) {
        if ((*it).nick == cmd[0]) {
            if (cli.cliFd == cha.op->cliFd)
                Utilities::writeRpl((*it).cliFd, RPL_INVITE(cli.nick, (*it).ipAddr, cmd[1], cmd[0]));
            else
                Utilities::writeRpl(cli.cliFd, ERR_CHANOPRIVSNEEDED(cli.nick, cmd[1]));
            break;
        }
    }
    return 1;
}
