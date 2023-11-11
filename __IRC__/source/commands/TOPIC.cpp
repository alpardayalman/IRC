#include "Server.hpp"

int     Server::Topic(std::string &s, Client &cli) {

    std::stringstream ss(s);
    std::string str, cha;
    ss >> cha;
    str = s.erase(0, cha.length());
    Utilities::trim(cha);
    if(cha == "ft_irc") { // calisiyormu test et
        cli.messageBox.push_back("You cannot change ft_irc");
        FD_SET(cli.cliFd, &this->writeFds);
    }
    else {
        for (ChanelIterator it = this->chanels.begin(); it != this->chanels.end(); ++it) {
            if (it->name == cha) {
                Utilities::writeAllRpl(this->getFds(), RPL_TOPIC(cli.nick, cli.ipAddr, cha, &str[1]));
                it->topic = &str[1];
                break;
            }
        }
    }

    return(1);
}
