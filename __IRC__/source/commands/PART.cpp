#include "Server.hpp"

int Server::Part(std::string &s, Client &cli) {
    std::stringstream ss(s);
    ss >> s;

    std::string msg;
    ChanelIterator it = this->chanels.begin();
    for (;it != this->chanels.end(); ++it)
    {
        if (it->name == s){ // channel adi cikmak istedigim chanel ise.
            for (ClientIterator cit = it->clients.begin(); cit != it->clients.end(); ++cit)
            {
                if (cit->nick == cli.nick) {
                    if (it->clients.size() == 1) {
                        this->chanels.erase(it); // bakilir.
                        return 0;
                    }
                    else {
                        if(cit->nick == it->op->nick) {
                            it->op = &it->clients[0];
                        }
                        it->clients.erase(cit);
                    }
                    showRightGui(cli, getChanel(s));
                    Utilities::writeRpl(cli.cliFd, RPL_PART(cli.getPrefix(), s));
                return 1;
                }

            }
        }
    }
    return 0;
}
