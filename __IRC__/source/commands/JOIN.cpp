#include "Server.hpp"
#include "stdio.h"

int  findChanel(std::string& name, std::vector<Chanel> chanels) {
    for (ChanelIterator it = chanels.begin(); it != chanels.end(); it++) {
        if (name == (*it).name)
            return 1;
    }
    return 0;
}

void showClients(Chanel &cha) {
    for (ClientIterator it = cha.clients.begin(); it != cha.clients.end(); it++) {
        std::cout << (*it).nick << "|";
    }
    std::cout << std::endl;
}

int Server::isClientIn(Chanel &cha, int cliFd) {
    for (ClientIterator it = cha.clients.begin(); it != cha.clients.end(); it++) {
        if ((*it).cliFd == cliFd)
            return 1;
    }
    return 0;
}
/*
    client ve bir channel name atiliyor.
    sonra clientin icindeki channellerde dolasilip eger client'in icinde oldugu channel
    varsa true donduruyor.
*/
int Server::isClientIn(Client& cli, std::string nameChanel) {
    int ret = 1;
    for (ChanelIterator it = this->chanels.begin(); it != this->chanels.end(); ++it, ++ret) {
        if ((*it).name == nameChanel) {
            for (ClientIterator cit = (*it).clients.begin(); cit != (*it).clients.end(); ++cit) {
                if ((*cit).cliFd == cli.cliFd)
                    return ret;
            }
            break;
        }
    }
    return 0;
}

int    Server::Join(std::string &s, Client& cli) {
    std::stringstream ss(s);
    std::string chaName, key;
    std::string msg;
    if (!s.empty()) {
        ss >> chaName;
        ss >> key;
        if(!Utilities::checkChannel(chaName)) {
            // write(cli.cliFd, "Channel name must be" ,20); // bool yapicaz unutmaa!!!!!!!!!!
            return 0;
        }
            if (findChanel(chaName, this->chanels)) {
                for (ChanelIterator it = chanels.begin(); it != chanels.end(); ++it) {
                    if (chaName == (*it).name) {//if chanel exist try ot join chanel
                        if (!isClientIn((*it), cli.cliFd)) {
                            (*it).clients.push_back(cli);
                            std::string str = RPL_JOIN(cli.nick, cli.ipAddr, chaName);
                            write(cli.cliFd, str.c_str(), str.length());

                        }//client is in chanel or not
                        else {
                            cli.messageBox.push_back("You already in the chanel");
                            FD_SET(cli.cliFd, &this->writeFds);
                        }
                    }
                }   
            } else {//if chanel does not exist, create one and add the client to the chanel vector.
                Chanel  newChanel(chaName, cli);
                newChanel.op = cli;
                this->chanels.push_back(newChanel); // Chanels cached in server.
                std::string str = RPL_JOIN(cli.nick, cli.ipAddr, chaName);
                write(cli.cliFd, str.c_str(), str.length());
                this->chanels.back().clients.push_back(cli);
            }
            for(std::vector<Client>::iterator it = this->clients.begin() ; it != this->clients.end(); ++it) {
                if(int chidx = isClientIn((*it), chaName)) {
                    if (it->cliFd == this->chanels[chidx-1].op.cliFd)
                        msg += "@";
                    msg += (*it).nick + " ";
                }
            }
            for(std::vector<Client>::iterator it = this->clients.begin() ; it != this->clients.end(); ++it) {
                std::string str = RPL_NAMREPLY(cli.nick, chaName, msg);
                write((*it).cliFd, str.c_str(), str.length());
                str = RPL_ENDOFNAMES(cli.nick, chaName);
                write((*it).cliFd, str.c_str(), str.length());
            }
    } else {//if join command input empty write this message
        cli.messageBox.push_back("you cannot joined chanel cause use command correct JOIN <chanel name>");
        FD_SET(cli.cliFd, &this->writeFds);
    }
    return 0;
}