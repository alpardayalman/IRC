#include "Server.hpp"



int     Server::PrivMsg(std::string &s, Client& cli) {
    //PRIVMSG #<chanel name> :<msg>
    std::vector<std::string> params = Utilities::tokenCmd(s, 0); // cmd[0] eger chanel ise basinda # olucak client ise olmayacak.
    if (!cli.passcheku)
        return 0;
    for(std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end();++it)
    {
        if (it->cliFd != cli.cliFd && isClientIn((*it), params[0]))
        {
            (*it).messageBox.push_back(RPL_PRIVMSG(cli.getPrefix(), params[0], params[1].substr(1, params[1].length())));
            FD_SET((*it).cliFd, &this->writeFds);
        }
        else if (params[0] == (*it).user || params[0] == (*it).nick) {//PRIVMSG <user> <msg> PRIVMSG <nick> <msg>
            if (params[1].find("PING") != std::string::npos) {
                s = params[0] + " " + s + " " +cli.nick;
                std::cout << PURPLE << it->nick  << WHITE << std::endl;
                Server::Ping(s, (*it));
                break;
            }
            (*it).messageBox.push_back(RPL_PRIVMSG(cli.getPrefix(), (*it).nick, params[1].substr(1, params[1].length())));
            FD_SET((*it).cliFd, &this->writeFds);
        }
    }
    return 1;
}