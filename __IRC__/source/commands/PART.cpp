#include "Server.hpp"



// server -> chanel -> user silinecek.
// /kick #test1 ayalman why not
int Server::Part(std::string &s, Client &cli) {
    std::stringstream ss(s);
    ss >> s;
    cli.messageBox.push_back(RPL_PART(cli.getPrefix(), s));
    FD_SET(cli.cliFd, &this->writeFds);
    
    for (ChanelIterator it = this->chanels.begin(); it != this->chanels.end(); ++it) {
        if (it->name == s){
            for (ClientIterator cit = it->clients.begin(); cit != it->clients.end(); ++cit) {
                if (cit->nick == cli.nick) {
                    it->clients.erase(cit);
                    break;
                }
            }
        }
    }
    return 1;
}
