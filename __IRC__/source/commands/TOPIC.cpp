#include "Server.hpp"

int     Server::Topic(std::string &s, Client &cli) {
    std::cout << "\ntopic: " << s << std::endl;
    std::stringstream ss(s);
    std::string str, cha;
    ss >> cha;
    str = s.erase(0, cha.length());
    Utilities::trim(cha);
    std::cout << __LINE__ << std::endl;
    if(cha == "ft_irc") { // calisiyormu test et
        cli.messageBox.push_back("You cannot change ft_irc");
        FD_SET(cli.cliFd, &this->writeFds);
    }
    else {
        std::cout << __LINE__ << std::endl;
        for (ChanelIterator it = this->chanels.begin(); it != this->chanels.end(); ++it) {
            std::cout << __LINE__ << std::endl;
            if (it->name == cha) {
                std::cout << __LINE__ << std::endl;
                for (ClientIterator cit = it->clients.begin(); cit != it->clients.end(); ++cit) {
                    std::cout << __LINE__ << std::endl;
                    // cit->messageBox.push_back(RPL_TOPIC(cli.nick, cit->ipAddr, cha, &str[1]));
                    // FD_SET(cit->cliFd, &this->writeFds);
                    std::string stru = RPL_TOPIC(cli.nick, cit->ipAddr, cha, &str[1]);
                    write(cit->cliFd, stru.c_str(), stru.length());
                }
                break;
            }
        }
    }

    return(1);
}
