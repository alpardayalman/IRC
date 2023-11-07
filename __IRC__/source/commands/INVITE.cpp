#include "Server.hpp"

int Server::Invite(std::string &s, Client &cli) {
    std::cout << "\nINV: " << s << std::endl;
    std::vector<std::string> cmd = Utilities::tokenCmd(s, 1);

    for (std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end(); ++it) {
        std::cout << "itin: " << (*it).nick << " trnin: " << cmd[0] << std::endl;

        if ((*it).nick == cmd[0]) {
            std::string pri = RPL_INVITE(cli.nick, (*it).ipAddr, cmd[1], cmd[0]);
            write((*it).cliFd, pri.c_str(), pri.size());
        }
    }
    return 1;
}
