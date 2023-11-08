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

int Server::isClientIn(Client& cli, std::string nameChanel) {
    for (ChanelIterator it = cli.connectedChanels.begin(); it != cli.connectedChanels.end(); ++it) {
        if ((*it).name == nameChanel)
            return 1;
    }
    return 0;
}

int    Server::Join(std::string &s, Client& cli) {
    std::stringstream ss(s);
    std::string chaName, key;
    if (!s.empty()) {
        ss >> chaName;
        ss >> key;
        if(!Utilities::checkChannel(chaName)) {
            // write(cli.cliFd, "Channel name must be" ,20); // bool yapicaz unutmaa!!!!!!!!!!
            return 0;
        }
            if (findChanel(chaName, this->chanels)) {
                for (ChanelIterator it = chanels.begin(); it != chanels.end(); it++) {
                    if (chaName == (*it).name) {//if chanel is exist try ot join chanel
                        if (!isClientIn((*it), cli.cliFd)) {
                            (*it).clients.push_back(cli);
                            cli.connectedChanels.push_back((*it));
                            cli.messageBox.push_back(RPL_JOIN(cli.nick, cli.ipAddr, chaName));
                            FD_SET(cli.cliFd, &this->writeFds);
                        }//client is in chanel or not
                        else
                            cli.messageBox.push_back("You already in the chanel");
                            FD_SET(cli.cliFd, &this->writeFds);
                    }
                }
            } else {//if chanel does not exist, create one and add the client to the chanel vector.
                Chanel  newChanel(chaName);
                this->chanels.push_back(newChanel); // Chanels cached in server.
                cli.connectedChanels.push_back(newChanel); // Clients that are connected to the channels.
                cli.messageBox.push_back(RPL_JOIN(cli.nick, cli.ipAddr, chaName));
                FD_SET(cli.cliFd, &this->writeFds);
                this->chanels.back().clients.push_back(cli);
            }
    } else {//if join command input empty write this message
        cli.messageBox.push_back("you cannot joined chanel cause use command correct JOIN <chanel name>");
        FD_SET(cli.cliFd, &this->writeFds);
    }
    return 0;
}