#include "Server.hpp"



// server -> chanel -> user silinecek.
// /kick #test1 ayalman why not
int Server::Part(std::string &s, Client &cli) {
    std::stringstream ss(s);
    ss >> s;

    std::string msg; // yanlarda ad silinsin diye.
    ChanelIterator it = this->chanels.begin();
    for (; it != this->chanels.end(); ++it) {
        if (it->name == s){
            for (ClientIterator cit = it->clients.begin(); cit != it->clients.end(); ++cit) {
                if (cit->nick == cli.nick) {
                    it->clients.erase(cit);
                    break;
                }
                else {
                    msg += (*cit).user + " "; // operator eklenilecek.
                }
            }
            break;
        }
    }
    cli.messageBox.push_back(RPL_PART(cli.getPrefix(), s));
    FD_SET(cli.cliFd, &this->writeFds);

    for(std::vector<Client>::iterator cit = it->clients.begin() ; cit != it->clients.end(); ++cit) {
        std::string str = RPL_NAMREPLY(cli.nick, it->name, msg);
        write((*cit).cliFd, str.c_str(), str.length());
        str = RPL_ENDOFNAMES(cli.nick, it->name);
        write((*cit).cliFd, str.c_str(), str.length());
    }
    return 1;
}
