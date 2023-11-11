#include "Server.hpp"
//ha buraya namerpl_

void showUsers(Chanel &cha) {
    std::cout << PURPLE;
    for (ClientIterator it = cha.clients.begin(); it != cha.clients.end(); it++) {
        std::cout << it->nick << " ";
    }
    std::cout << RESET << std::endl;
}

/*
    Client'in positionunu verir.
    Eger bulamazsa -1 verir.
*/
int getClientPos(Chanel &cha, Client &cli) {
    for (int i = 0; (int)cha.clients.size() > i; i++) {
        if (cli.nick == cha.clients[i].nick)
            return i;
    }
    return -1;
}

int Server::Op(std::string &s, Client &cli) {//  s = <chanelname> +o <nickname>
    std::vector<std::string> cmd = Utilities::splitString(s);
    Chanel cha = getChanel(cmd[0]);
    if (cli.nick == cha.op->nick) {//komutu gonderen client eger op ise
        Utilities::trim(cmd[2]);
        Client newOp = cha.getClient(cmd[2]);
        Client oldOp = cha.getClient(cli.nick);
        if (isClientIn(oldOp, cmd[0]) && isClientIn(newOp, cmd[0])) {//eger oldOperator ve newOperator ayni kanalda ise
            for (ChanelIterator it = chanels.begin(); it != chanels.end(); it++) {
                if (cmd[0] == it->name) {
                    //inputtan gelen chanel name iteratorun chanelNameine esitse new ve olp op'u swapliyor
                    Client tmp = it->clients[getClientPos(*it, oldOp)];
                    it->clients[getClientPos(*it, oldOp)] = it->clients[getClientPos(*it, newOp)];
                    it->clients[getClientPos(*it, newOp)] = tmp;
                    it->op = &it->clients[getClientPos(*it, newOp)];
                }
            }
        }
        std::string msg;
        for(std::vector<Client>::iterator it = cha.clients.begin() ; it != cha.clients.end(); ++it) {
                if(int chidx = isClientIn((*it), cmd[0])) {
                    if (it->cliFd == this->chanels[chidx-1].op->cliFd)
                        msg += "@";
                    msg += (*it).nick + " ";
                }
            }
        Utilities::writeAllRpl(this->getFds(), RPL_NAMREPLY(cli.nick, cmd[0], msg));
        Utilities::writeAllRpl(this->getFds(), RPL_ENDOFNAMES(cli.nick, cmd[0]));
    } else {//find(rpl_erroryournotoperator())
    }
    return 1;
}
