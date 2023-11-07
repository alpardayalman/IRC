#include "Server.hpp"



int     Server::PrivMsg(std::string &s, Client& cli) {
    //PRIVMSG #<chanel name> :<msg>
    std::vector<std::string> params = Utilities::tokenCmd(s); // cmd[0] eger chanel ise basinda # olucak client ise olmayacak.
    if (!cli.passcheku)
        return 0;
    for(std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end();++it)
    {
        if (it->cliFd != cli.cliFd && isClientIn((*it), params[0]))
        {
            std::cout << params[0] << "PARAMS" << std::endl;
            (*it).messageBox.push_back(RPL_PRIVMSG(cli.getPrefix(), params[0], params[1].substr(1, params[1].length())));
            FD_SET((*it).cliFd, &this->writeFds);
        }
    }
    for (std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end(); ++it) {
        if (params[0] == (*it).user || params[0] == (*it).nick) {
            std::cout << "param:" << params[0] << "|user:" << (*it).user << std::endl;
            std::string a = RPL_PRIVMSG(cli.getPrefix(), (*it).nick, params[1].substr(1, params[1].length()));
            write((*it).cliFd, a.c_str(), a.size());
            // std::cout << params[1] << std::endl;
            // Utilities::fd_write_color(1, a, PURPLE);
            (*it).messageBox.push_back(RPL_PRIVMSG(cli.getPrefix(), (*it).nick, params[1].substr(1, params[1].length())));
        }
    }
    return 1;
}