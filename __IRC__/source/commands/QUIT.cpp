#include "Server.hpp"

int Server::Quit(std::string &s, Client &cli) {

    for (ChanelIterator it = this->chanels.begin(); it != this->chanels.end(); ++it) {
        Server::Part(it->name, cli);
	}
    for (ClientIterator it = this->clients.begin(); it != this->clients.end(); ++it) {
        if (it->nick == cli.nick) {
            std::cout << YELLOW << "Client " << cli.nick << " has left the server." << RESET << std::endl;
            this->clients.erase(it);
            break;
        }
    }
    Utilities::writeRpl(cli.cliFd, RPL_QUIT(cli.getPrefix(), s.c_str()));
    close(cli.cliFd);
    return 1;
}