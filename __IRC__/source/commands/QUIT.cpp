#include "Server.hpp"

int Server::Quit(std::string &s, Client &cli) {

    for (ChanelIterator it = this->chanels.begin(); it != this->chanels.end(); ++it) {
        Server::Part(it->name, cli);
	}
    Utilities::writeRpl(cli.cliFd, RPL_QUIT(cli.getPrefix(), s.c_str()));
    close(cli.cliFd);
    return 1;
}