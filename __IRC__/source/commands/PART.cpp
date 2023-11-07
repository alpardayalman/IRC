#include "Server.hpp"



// server -> chanel -> user silinecek.
// /kick #test1 ayalman why not
int Server::Part(std::string &s, Client &cli) {
    std::stringstream ss(s);
    ss >> s;
    std::string pri = RPL_PART(cli.getPrefix(), s);
    write(cli.cliFd, pri.c_str(), pri.length());
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
