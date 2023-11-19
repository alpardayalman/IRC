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
        if(cli.nick.empty() || cli.user.empty()) {
            cli.messageBox.push_back("You cannot joined chanel cause you don't have a nickname or username\n");
            FD_SET(cli.cliFd, &this->writeFds);
            return 0;
        }
        ss >> chaName;
        ss >> key;
        std::cout << PURPLE << "s: " << s << " chaName: " << chaName << " name: " << cli.nick << RESET << std::endl;
        if(!Utilities::checkChannel(chaName)) {
            Utilities::writeRpl(cli.cliFd, ERR_NOSUCHCHANNEL(cli.getPrefix(), chaName));
            return 0;
        }
        if (findChanel(chaName, this->chanels)) {
            for (ChanelIterator it = chanels.begin(); it != chanels.end(); ++it) {
                if (chaName == (*it).name) {//if chanel exist try ot join chanel
                    if (!isClientIn((*it), cli.cliFd)) {
                        if ((it->keycode & K_CODE) && (it->keycode & L_CODE))
                        {
                            if (it->users <= (int) it->clients.size()) {
                                Utilities::writeRpl(cli.cliFd, ERR_CHANNELISFULL(cli.nick, chaName));
                            }
                            else if (it->key != key) {
                                Utilities::writeRpl(cli.cliFd, ERR_BADCHANNELKEY(cli.nick, chaName));
                            }
                            else {
                                (*it).clients.push_back(cli);
                                Utilities::writeRpl(cli.cliFd, RPL_JOIN(cli.nick, cli.ipAddr, chaName));
                                if (!(*it).topic.empty())
                                    Utilities::writeRpl(cli.cliFd, RPL_TOPIC(cli.nick, cli.ipAddr, chaName, (*it).topic));
                            }
                        }
                        else if ((it->keycode & K_CODE)) {
                            if (it->key != key) {
                                Utilities::writeRpl(cli.cliFd, ERR_BADCHANNELKEY(cli.nick, chaName));
                            }
                            else {
                                (*it).clients.push_back(cli);
                                Utilities::writeRpl(cli.cliFd, RPL_JOIN(cli.nick, cli.ipAddr, chaName));
                                if (!(*it).topic.empty())
                                    Utilities::writeRpl(cli.cliFd, RPL_TOPIC(cli.nick, cli.ipAddr, chaName, (*it).topic));
                            }
                        }
                        else if ((it->keycode & L_CODE)) {
                            if (it->users <= (int) it->clients.size()) {
                                Utilities::writeRpl(cli.cliFd, ERR_CHANNELISFULL(cli.nick, chaName));
                            }
                            else {
                                (*it).clients.push_back(cli);
                                Utilities::writeRpl(cli.cliFd, RPL_JOIN(cli.nick, cli.ipAddr, chaName));
                                if (!(*it).topic.empty())
                                    Utilities::writeRpl(cli.cliFd, RPL_TOPIC(cli.nick, cli.ipAddr, chaName, (*it).topic));
                            }
                        }
                        else {
                            (*it).clients.push_back(cli);
                            Utilities::writeRpl(cli.cliFd, RPL_JOIN(cli.nick, cli.ipAddr, chaName));
                            if (!(*it).topic.empty())
                                Utilities::writeRpl(cli.cliFd, RPL_TOPIC(cli.nick, cli.ipAddr, chaName, (*it).topic));
                        }

                    }//client is in chanel or not
                    else {
                        cli.messageBox.push_back("You already in the chanel");
                        FD_SET(cli.cliFd, &this->writeFds);
                    }
                    break;
                }
            }
        }
        else {//if chanel does not exist, create one and add the client to the chanel vector.
            Chanel  newChanel(chaName);
            // newChanel.op = &cli;
            newChanel.clients.push_back(cli);
            newChanel.op = &newChanel.clients[0];
            this->chanels.push_back(newChanel); // Chanels cached in server.
            Utilities::writeRpl(cli.cliFd, RPL_JOIN(cli.nick, cli.ipAddr, chaName));
        }
        showRightGui(cli, getChanel(chaName));
    } 
    else {//if join command input empty write this message
        cli.messageBox.push_back("you cannot joined chanel cause use command correct JOIN <chanel name>");
        FD_SET(cli.cliFd, &this->writeFds);
    }
    return 0;
}
