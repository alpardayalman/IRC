#include "Server.hpp"

int     Server::Topic(std::string &s, Client &cli) {
    std::cout << "\ntopic: " << s << std::endl;
    std::stringstream ss(s);
    std::string str, cha;
    ss >> cha;
    str = s.erase(0, cha.length() + 1);
    Utilities::trim(cha);
    if(cha == "ft_irc") {
        write(cli.cliFd, "You cannot change ft_irc", 25);
    }
    else {
        for (ChanelIterator it = chanels.begin(); it != chanels.end(); ++it) {
            if (it->name == cha) {
                for (ClientIterator cit = it->clients.begin(); cit != it->clients.end(); ++cit) {
                    std::string sendi = RPL_TOPIC(cli.nick, cit->ipAddr, cha, &str[1]);
                    write(cit->cliFd, sendi.c_str(), sendi.size());
                }
            }
        }
    }

    return(1);
}
