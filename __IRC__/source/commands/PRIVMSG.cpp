#include "Server.hpp"



int     Server::PrivMsg(std::string &s, Client& cli) {
    //PRIVMSG #<chanel name> :<msg>
    std::vector<std::string> params = Utilities::tokenCmd(s);
    if (!cli.passcheku)
        return 0;
    for(std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end();++it)
    {
        if (it->cliFd != cli.cliFd && isClientIn((*it), params[0]))
        {
            (*it).messageBox.push_back(s+'\n');
            FD_SET((*it).cliFd, &this->writeFds);
        }
    }
    return 1;
}