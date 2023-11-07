#include "Server.hpp"



// server -> chanel -> user silinecek.
int Server::Kick(std::string &s, Client &cli) {
    std::vector<std::string> cmd = Utilities::tokenCmd(s);
    std::string kick = Utilities::trim(cmd[0]);
    std::string parse = Utilities::trim(cmd[1]);
    cmd = Utilities::tokenCmd(parse);
    std::string kname = Utilities::trim(cmd[0]);
    std::string reason = Utilities::trim(cmd[1]); // 1 fazlasindan baslayip yazdiricaz
    for (std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end(); ++it) {
        if ((*it).nick == kname) {
            std::string pri = RPL_KICK(cli.nick, kick, kname, reason);
            write((*it).cliFd, pri.c_str(), pri.size());
            for(std::vector<Chanel>::iterator it = this->chanels.begin(); it != this->chanels.end(); ++it) {
                if((*it).name == kick) {
                    for(std::vector<Client>::iterator it2 = (*it).clients.begin(); it2 != (*it).clients.end(); ++it2) {
                        if((*it2).nick == kname) {
                            (*it).clients.erase(it2);
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }

    std::cout << "\nkick = " << kick << " kname = " << kname << " reason = " << reason << std::endl;
    return 1;
}

//KICK #asdf ola