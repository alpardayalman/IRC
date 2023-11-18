#include "Server.hpp"

int Server::Quit(std::string &s, Client &cli) {
    for (ChanelIterator it = this->chanels.begin(); it != this->chanels.end(); ++it) {
        std::cout << PURPLE << it->name << RESET << std::endl;
        Server::Part(it->name, cli);
        if (it == this->chanels.end())
            break;
	}
    for (ClientIterator it = this->clients.begin(); it != this->clients.end(); ++it) {
        if (it->nick == cli.nick) {
            std::cout << YELLOW << "Client " << cli.nick << " has left the server." << RESET << std::endl;
            this->clients.erase(it);
            break;
        }
    }
    if (FD_ISSET(cli.cliFd, &this->writeFds))
        FD_CLR(cli.cliFd, &this->writeFds);
    if (FD_ISSET(cli.cliFd, &this->readFds))
        FD_CLR(cli.cliFd, &this->readFds);
    Utilities::writeRpl(cli.cliFd, RPL_QUIT(cli.getPrefix(), s.c_str()));
    close(cli.cliFd);
    return 1;
}
