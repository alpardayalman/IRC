#include "Server.hpp"

int Server::Quit(std::string &s, Client &cli) {
    for (ChanelIterator it = cli.connectedChanels.begin(); it != cli.connectedChanels.end(); ++it) {
        Server::Part(it->name, cli);
	}
    s = RPL_QUIT(cli.getPrefix(), s.c_str());
    write(cli.cliFd, s.c_str(), s.length());
    return 1;
}