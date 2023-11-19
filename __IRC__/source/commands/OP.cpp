#include "Server.hpp"

void showUsers(Chanel &cha) {
    std::cout << PURPLE;
    for (ClientIterator it = cha.clients.begin(); it != cha.clients.end(); it++) {
        std::cout << it->nick << " ";
    }
    std::cout << RESET << std::endl;
}

int getClientPos(Chanel &cha, Client &cli) {
    for (int i = 0; (int)cha.clients.size() > i; i++) {
        if (cli.nick == cha.clients[i].nick)
            return i;
    }
    return -1;
}

int Server::Op(std::string &s, Client &cli) {
    std::vector<std::string> cmd = Utilities::splitString(s);
    Chanel cha = getChanel(cmd[0]);
    if (cli.nick == cha.op->nick) {//if he is op
        Utilities::trim(cmd[2]);
        ClientIterator it_ = this->clients.begin();
        for (; it_ != this->clients.end(); ++it_) {
            if (it_->nick == cmd[2])
                break;
        }
        if (it_ == this->clients.end())
            return 1;
        Client newOp = cha.getClient(cmd[2]);
        Client oldOp = cha.getClient(cli.nick);

        // std::cout << YELLOW << "cmd:" << cmd[2] << " s:" << s << RESET << std::endl;
        if (isClientIn(oldOp, cmd[0]) && isClientIn(newOp, cmd[0])) {//if oldop and newop same channel
            for (ChanelIterator it = chanels.begin(); it != chanels.end(); it++) {
                if (cmd[0] == it->name && getClientPos(*it, oldOp) != -1 && getClientPos(*it, newOp) != -1) {
                    //swap oldop and newop if they are in same chanel
                    Client tmp = it->clients[getClientPos(*it, oldOp)];
                    it->clients[getClientPos(*it, oldOp)] = it->clients[getClientPos(*it, newOp)];
                    it->clients[getClientPos(*it, newOp)] = tmp;
                    it->op = &it->clients[getClientPos(*it, newOp)];
                }
            }
        }
        Server::showRightGui(cli, cha);
    } else {
        Utilities::writeRpl(cli.cliFd, ERR_CHANOPRIVSNEEDED(cli.getPrefix(), cha.name));
    }
    return 1;
}
