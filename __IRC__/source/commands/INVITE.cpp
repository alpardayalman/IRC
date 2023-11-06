#include "Server.hpp"

int     Server::Invite(std::string &s, Client &cli) {
    std::cout << "\nINV: " << s << std::endl;
    std::vector<std::string> cmd = Utilities::tokenCmd(s);
    std::string tr = Utilities::trim(cmd[0]);
    std::string tr1 = Utilities::trim(cmd[1]);
    for(std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end(); ++it) {
        std::cout << "itin: " << (*it).nick << " trnin: " << tr << std::endl;
        if((*it).nick == tr) {
            std::cout << "LAAAN" << std::endl;
            std::string pri = RPL_INVITE(cli.nick, (*it).ipAddr, tr1, tr);
            write((*it).cliFd, pri.c_str(), pri.size());
        }
    }
    
    return 1;
}
