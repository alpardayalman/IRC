#include "Server.hpp"

int Server::Kick(std::string &s, Client &cli) {

    std::vector<std::string> cmd = Utilities::tokenCmd(s, 1); // 
    std::string kick = cmd[0];
    std::string parse = cmd[1];
    cmd = Utilities::tokenCmd(parse, 1); // cmd[0] kname : cmd[1] reason
    Chanel cha = getChanel(kick);
    std::cout << "KIck " << kick << " firstcmd0: " << cmd[0] << std::endl;
    // cok kotu bir kick bu.

    if(cha.op->nick == parse || cha.op->nick != cli.nick) {
        std::cout << RED << "you can not kick OP" << RESET << std::endl;
        return 1;
    }
    /*
     client iteratorunde gezip bir clientin nickini buluyoruz.
     sonra anyni adda bir chanel iteratoru yapip kickleme islemi yapmak istedigimiz chaneli buluyoruz.
     onun icinde birtane daha client iteratoru yapip kicklemek istedigimiz clienti o chanellden kickliyoruz.
    */
    for (std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end(); ++it) {
        std::cout << "*it.nick " << (*it).nick << " cmd[0] " << cmd[0] << std::endl;
        if ((*it).nick == cmd[0]) {
            (*it).messageBox.push_back(RPL_KICK(cli.nick, kick, cmd[0], cmd[1]));
            FD_SET((*it).cliFd, &this->writeFds);
            for(std::vector<Chanel>::iterator it3 = this->chanels.begin(); it3 != this->chanels.end(); ++it3) {
                if((*it3).name == kick) {
                    for(std::vector<Client>::iterator it2 = (*it3).clients.begin(); it2 != (*it3).clients.end(); ++it2) {
                        if((*it2).nick == cmd[0]) {
                            (*it3).clients.erase(it2);
                            showRightGui(cli, getChanel(kick));
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }
    return 1;
}
