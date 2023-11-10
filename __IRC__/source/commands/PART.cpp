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
            it->op = &it->clients[0];//uga buga adresler degismek iteratorda
            for (ClientIterator cit = it->clients.begin(); cit != it->clients.end(); ++cit) {
                if (cit->nick == cli.nick) {
                    if (it->op->nick == cit->nick) {
                        if (it->clients.size() > 1) {
                            ;
                        }
                    }
                    std::cout << &it->clients[0].nick << std::endl;
                    it->clients.erase(cit);
                    std::cout << &it->clients[0].nick << std::endl;
                    break;
                }
                else {
                    msg += (*cit).user + " "; // operator eklenilecek.
                }
            }
            if (!it->clients.size()) {
                this->chanels.erase(it);
            }
            break;
        }
    }
    cli.messageBox.push_back(RPL_PART(cli.getPrefix(), s));
    FD_SET(cli.cliFd, &this->writeFds);
    Utilities::writeAllRpl(this->getFds(), RPL_NAMREPLY(cli.nick, it->name, msg));
    Utilities::writeAllRpl(this->getFds(), RPL_ENDOFNAMES(cli.nick, it->name));
    return 1;
}
