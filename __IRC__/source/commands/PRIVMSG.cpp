#include "Server.hpp"



int     Server::PrivMsg(std::string &s, Client& cli) {
    //PRIVMSG #<chanel name> :<msg>
    std::cout << s << std::endl;
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
            std::string a = RPL_PRIVMSG(cli.getPrefix(), (*it).nick, params[1].substr(1, params[1].length())); // Buraya bakmak gerekiyor.
            write((*it).cliFd, a.c_str(), a.size());
            (*it).messageBox.push_back(RPL_PRIVMSG(cli.getPrefix(), (*it).nick, params[1].substr(1, params[1].length())));
        }
    }
    return 1;
}