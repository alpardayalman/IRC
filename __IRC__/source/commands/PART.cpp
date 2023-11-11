#include "Server.hpp"



// server -> chanel -> user silinecek.
// /kick #test1 ayalman why not
int Server::Part(std::string &s, Client &cli) {
    std::stringstream ss(s);
    ss >> s;

    std::string msg; // yanlarda ad silinsin diye.
    ChanelIterator it = this->chanels.begin();
    for (; it != this->chanels.end(); ++it) {
        if (it->name == s){ // Chanel bulunduysa
            for (ClientIterator cit = it->clients.begin(); cit != it->clients.end(); ++cit) { // chaneldeki clientlari gez
                if (cit->nick == cli.nick) {
                    if (it->op->nick == cit->nick) { // op ise
                        if (it->clients.size() > 1) {
                            it->op = &it->clients[0];//uga buga adresler degismek iteratorda
                        }
                        else {
                            this->chanels.erase(it);
                            it->op = NULL;
                        }
                    }    
                    it->clients.erase(cit);
                }
                if (cit == it->clients.end())
                    break;
            }
            for (ClientIterator cit = it->clients.begin(); cit != it->clients.end(); ++cit) {
                    if(it->op->nick == cit->nick)
                        msg += "@";
                    msg += (*cit).user + " ";
            }
                std::cout << RPL_NAMREPLY(cli.nick, it->name, msg) << std::endl;
                Utilities::writeAllRpl(this->getFds(), RPL_NAMREPLY(cli.nick, it->name, msg));
                Utilities::writeAllRpl(this->getFds(), RPL_ENDOFNAMES(cli.nick, it->name));
            break;
        }
    }
    std::cout << "chanel size: " << it->clients.size() << std::endl;
    Utilities::writeRpl(cli.cliFd, RPL_PART(cli.getPrefix(), s));

    return 1;
}
