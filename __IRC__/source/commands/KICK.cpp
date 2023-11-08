#include "Server.hpp"

int Server::Kick(std::string &s, Client &cli) {

    std::vector<std::string> cmd = Utilities::tokenCmd(s, 1); // 
    std::string kick = cmd[0];
    std::string parse = cmd[1];
    cmd = Utilities::tokenCmd(parse, 1); // cmd[0] kname : cmd[1] reason

    for (std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end(); ++it) {

        if ((*it).nick == cmd[0]) {
            (*it).messageBox.push_back(RPL_KICK(cli.nick, kick, cmd[0], cmd[1]));
            FD_SET((*it).cliFd, &this->writeFds);
            for(std::vector<Chanel>::iterator it = this->chanels.begin(); it != this->chanels.end(); ++it) {
                if((*it).name == kick) {
                    for(std::vector<Client>::iterator it2 = (*it).clients.begin(); it2 != (*it).clients.end(); ++it2) {
                        if((*it2).nick == cmd[0]) {
                            (*it).clients.erase(it2);
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }
    return 1;
}