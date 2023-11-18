#include "Server.hpp"

int     Server::Nick(std::string &s, Client &cli) {
    std::stringstream ss(s);
    ss >> s;
    if (!s.empty() && !isNickExist(s)) {
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
            showRightGui(cli, *it);
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
