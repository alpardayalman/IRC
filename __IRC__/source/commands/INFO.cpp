#include "Server.hpp"

int     Server::Info(std::string &s, Client& cli) {
    (void) s;
    Utilities::writeRpl(cli.cliFd, RPL_INFO(cli.nick, Utilities::infoMessage()));
    return 1;
    
}
