#include "Server.hpp"

int Server::Kick(std::string &s, Client &cli) {

    std::vector<std::string> cmd = Utilities::tokenCmd(s, 1); // 
    std::string kick = cmd[0];
    std::string parse = cmd[1];
    cmd = Utilities::tokenCmd(parse, 1); // cmd[0] kname : cmd[1] reason
    // cok kotu bir kick bu.

    /*
     client iteratorunde gezip bir clientin nickini buluyoruz.
     sonra anyni adda bir chanel iteratoru yapip kickleme islemi yapmak istedigimiz chaneli buluyoruz.
     onun icinde birtane daha client iteratoru yapip kicklemek istedigimiz clienti o chanellden kickliyoruz.
    */
    for (std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end(); ++it) {

        if ((*it).nick == cmd[0]) {
            (*it).messageBox.push_back(RPL_KICK(cli.nick, kick, cmd[0], cmd[1]));
            FD_SET((*it).cliFd, &this->writeFds);
            for(std::vector<Chanel>::iterator it = this->chanels.begin(); it != this->chanels.end(); ++it) {
                if((*it).name == kick) {
                    for(std::vector<Client>::iterator it2 = (*it).clients.begin(); it2 != (*it).clients.end(); ++it2) {
                        if((*it2).nick == cmd[0]) {
                            (*it).clients.erase(it2);
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }
    // std::string msg; // yanda adlar ciksin diye.
    // for (ChanelIterator it = this->chanels.begin(); it != this->chanels.end(); ++it) {
    //     if (it->name == kick) {
    //         for (ClientIterator cit = it->clients.begin(); cit != it->clients.begin(); ++cit) {
    //             if (cit->nick == cmd[0])
    //                 it->clients.erase(cit);
    //             else {
    //                 msg += (*cit).user + " ";
    //             }
    //         }
    //         for (ClientIterator cit = it->clients.begin(); cit != it->clients.begin(); ++cit) {
    //             std::string str = RPL_NAMREPLY(cli.nick, it->name, msg);
    //             write((*cit).cliFd, str.c_str(), str.length());
    //             str = RPL_ENDOFNAMES(cli.nick, it->name);
    //             write((*cit).cliFd, str.c_str(), str.length());   
    //         }
    //         break;
    //     }
    // }
    // for (ClientIterator it = this->clients.begin(); it != this->clients.end(); ++it) {
    //     if ((*it).nick == cmd[0]) {
    //         (*it).messageBox.push_back(RPL_KICK(cli.nick, kick, cmd[0], cmd[1]));
    //         FD_SET((*it).cliFd, &this->writeFds);
    //     }
    // }
    return 1;
}
