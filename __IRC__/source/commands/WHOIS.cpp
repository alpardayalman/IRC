#include "Server.hpp"

int Server::Whois(std::string &s, Client &cli) {

    std::vector<std::string> cmd = Utilities::tokenCmd(s, 0);
    std::string who = cmd[0];
    (void)cli;
    for(ClientIterator it = this->clients.begin(); it != this->clients.end(); ++it) {
        if (who == (*it).nick) {
            cli.messageBox.push_back(RPL_WHOISUSER((*it).nick, (*it).user, (*it).ip));
            FD_SET(cli.cliFd, &this->writeFds);
        }
    }
    std::cout << "who: " << who << std::endl;
    return 2147483647;
}