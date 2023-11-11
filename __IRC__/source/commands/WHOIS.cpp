#include "Server.hpp"

int Server::Whois(std::string &s, Client &cli) {

    std::vector<std::string> cmd = Utilities::tokenCmd(s, 0);
    std::string who = cmd[0];
    (void)cli;
    for(ClientIterator it = this->clients.begin(); it != this->clients.end(); ++it) {
        if (who == (*it).nick) {
            Utilities::writeRpl(cli.cliFd, RPL_WHOISUSER((*it).nick, (*it).user, (*it).ip));
            //bu satirda hangi kanallarda oldugunu yazdir.
            Utilities::writeRpl(cli.cliFd, RPL_WHOISSERVER((*it).nick, "masterbaiter"));
            Utilities::writeRpl(cli.cliFd, RPL_ENDOFWHOIS((*it).nick, (*it).nick));
            break;
        }
    }
    std::cout << "who: " << who << std::endl;
    return 1;
}