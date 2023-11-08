#include "Server.hpp"

int     Server::Topic(std::string &s, Client &cli) {
    std::cout << "\ntopic: " << s << std::endl;
    std::stringstream ss(s);
    std::string str, cha;
    ss >> cha;
    str = s.erase(0, cha.length() + 1);
    Utilities::trim(cha);
    if(cha == "ft_irc") { // calisiyormu test et
        cli.messageBox.push_back("You cannot change ft_irc");
        FD_SET(cli.cliFd, &this->writeFds);
    }
    else {
        for (ChanelIterator it = chanels.begin(); it != chanels.end(); ++it) {
            if (it->name == cha) {
                for (ClientIterator cit = it->clients.begin(); cit != it->clients.end(); ++cit) {
                    cli.messageBox.push_back(RPL_TOPIC(cli.nick, cit->ipAddr, cha, &str[1]));
                    FD_SET(cit->cliFd, &this->writeFds);
                }
            }
        }
    }

    return(1);
}
