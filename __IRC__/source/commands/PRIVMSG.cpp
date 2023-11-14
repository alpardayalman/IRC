#include "Server.hpp"



int     Server::PrivMsg(std::string &s, Client& cli) {
    //PRIVMSG #<chanel name> :<msg>
    std::vector<std::string> params = Utilities::tokenCmd(s, 0); // cmd[0] eger chanel ise basinda # olucak client ise olmayacak.
    if (!cli.passcheku)
        return 0;
    for(std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end();++it) // tum clientlarda geziyoruz.
    {
        if (it->cliFd != cli.cliFd && isClientIn((*it), params[0]))// eger kendin degil isen ve client(it) bu chanelde ise
        {
            if (!isClientIn(cli, params[0])) // sen chanelde misin.
                return 1;
            (*it).messageBox.push_back(RPL_PRIVMSG(cli.getPrefix(), params[0], (params[1][0]==':'?params[1].substr(1, params[1].length()) : params[1])));
            FD_SET((*it).cliFd, &this->writeFds);
        }
        else if (params[0] == (*it).user || params[0] == (*it).nick) {//PRIVMSG <user> <msg> PRIVMSG <nick> <msg>
            if (params[1].find("PING") != std::string::npos) {
                s = s + " " +cli.nick;
                Server::Ping(s, cli);
                break;
            }
            (*it).messageBox.push_back(RPL_PRIVMSG(cli.getPrefix(), (*it).nick, (params[1][0]==':'?params[1].substr(1, params[1].length()) : params[1])));
            FD_SET((*it).cliFd, &this->writeFds);
        }
    }
    return 1;
}
