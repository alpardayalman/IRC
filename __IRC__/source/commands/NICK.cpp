#include "Server.hpp"

int     Server::Nick(std::string &s, Client &cli) {
    std::stringstream ss(s);
    ss >> s;
    std::cout << s << std::endl;
    if (!s.empty() && !isNickExist(s)) { // bos degil ve nick 
        cli.messageBox.push_back(RPL_NICK(cli.nick, cli.user, cli.ip, s));
        FD_SET(cli.cliFd, &this->writeFds);
        for (ChanelIterator it = this->chanels.begin(); it != this->chanels.end(); ++it) {
            std::string msg;
            for (ClientIterator cit = (*it).clients.begin(); cit != (*it).clients.end(); ++cit) {
                if (cli.nick == cit->nick)
                {
                    cit->nick = s;
                    break;
                }
            }
            for(std::vector<Client>::iterator cit = (*it).clients.begin() ; cit != (*it).clients.end(); ++cit) {
                if(int chidx = isClientIn((*cit), it->name)) {
                    if (cit->cliFd == this->chanels[chidx-1].op->cliFd)
                        msg += "@";
                    msg += (*cit).nick + " ";
            }
            Utilities::writeAllRpl(it->getFds(), RPL_NAMREPLY(cli.nick, it->name, msg));
            Utilities::writeAllRpl(it->getFds(), RPL_ENDOFNAMES(cli.nick, it->name));
        }
        }
        cli.nick = s;
    }
    else {
        if(s.empty())
            Utilities::writeRpl(cli.cliFd, ERR_NICKNAMEEMPTY(cli.getPrefix()));
        else
            Utilities::writeRpl(cli.cliFd, ERR_NICKNAMEINUSE(cli.getPrefix()));
    }
    return 0;
}
