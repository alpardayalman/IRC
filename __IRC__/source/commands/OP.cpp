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
    std::cout << PURPLE << "cmd[0]: " << cmd[0] << " cmd[1]: " << cmd[1] << " cmd[2]: " << cmd[2] << RESET << std::endl;
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
        if (oldOp.nick.empty() || newOp.nick.empty()) {
            std::cout << RED << "oldOp or newOp is empty" << RESET << std::endl;
            return 1;
        }

        if (isClientIn(oldOp, cmd[0]) && isClientIn(newOp, cmd[0])) {//if oldop and newop same channel
            int posOldOp = getClientPos(cha, oldOp);
            int posNewOp = getClientPos(cha, newOp);
            std::cout << PURPLE << "posOldOp: " << posOldOp << " " << " posNewOp: " << posNewOp << RESET << std::endl;
            for (ChanelIterator it = chanels.begin(); it != chanels.end(); it++) {
                if (cmd[0] == it->name && posOldOp != -1 && posNewOp != -1) {
                    //swap oldop and newop if they are in same chanel
                    Client tmp = it->clients[0];
                    it->clients[0] = it->clients[posNewOp];
                    it->clients[posNewOp] = tmp;
                    // it->clients[getClientPos(*it, oldOp)] = it->clients[getClientPos(*it, newOp)];
                    // it->clients[getClientPos(*it, newOp)] = tmp;
                    it->op = &it->clients[0];
                }
            }
        }
        Server::showRightGui(cli, cha);
    } else {
        Utilities::writeRpl(cli.cliFd, ERR_CHANOPRIVSNEEDED(cli.getPrefix(), cha.name));
    }
    return 1;
}
