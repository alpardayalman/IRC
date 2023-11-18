#include "Server.hpp"

int Server::Kick(std::string &s, Client &cli) {
    if (s[0] != '#') // so that you can't kick someone in private chat.
        return 0;
    std::vector<std::string> cmd = Utilities::splitString(s); // 
    std::string kick = cmd[0];
    std::string parse = cmd[1];

    Chanel cha = getChanel(cmd[0]);
    if (cha.name.empty()){
        return 0;
    }

    if(cha.op->nick == cmd[1] || cha.op->nick != cli.nick) {
        if(cha.op->nick != cli.nick) {
            Utilities::writeRpl(cli.cliFd, ERR_CHANOPRIVSNEEDED(cli.getPrefix(), kick));
        }
        else
            Utilities::writeRpl(cli.cliFd, ERR_CHANOPKICK(cli.getPrefix(), kick));
        return 1;
    }

    for (ClientIterator kit = this->clients.begin(); kit != this->clients.end(); ++kit) {
        if (kit->nick ==  cmd[1]) {
            kit->messageBox.push_back(RPL_KICK(cli.nick, kick, cmd[1], (cmd.size() >= 3 ? (cmd[2][0] == ':' ? cmd[2].substr(1, cmd[2].length()) : cmd[2]) : "No reason needed")));
            FD_SET(kit->cliFd, &this->writeFds);
            break;
        }
    }
    for (ChanelIterator it = this->chanels.begin(); it != this->chanels.end(); ++it) {
        if (it->name == kick) {
            for (ClientIterator cit = it->clients.begin(); cit != it->clients.end(); ++cit) {
                if (cit->nick == cmd[1]) {
                    it->clients.erase(cit);
                    showRightGui(cli, getChanel(kick));
                    return 1;
                }
            }
            break;
        }
    }
    return 1;
}
