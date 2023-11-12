#include "Server.hpp"

int Server::Part(std::string &s, Client &cli) {
    std::stringstream ss(s);
    ss >> s;

    std::string msg;
    ChanelIterator it = this->chanels.begin();
    for (; it != this->chanels.end(); ++it) {
        if (it->name == s){
            for (ClientIterator cit = it->clients.begin(); cit != it->clients.end(); ++cit) {
                if (cit->nick == cli.nick) {
                    if (it->op->nick == cit->nick) {
                        if (it->clients.size() > 1) {
                            it->op = &it->clients[0];
                        }
                        else {
                            it->clients.erase(cit);
                            this->chanels.erase(it);
                            it->op = NULL;
                            break;
                        }
                    }    
                    it->clients.erase(cit);
                }
                if (cit == it->clients.end())
                    break;
            }
            showRightGui(cli, getChanel(s));
            break;
        }
    }
    Utilities::writeRpl(cli.cliFd, RPL_PART(cli.getPrefix(), s));

    return 1;
}
