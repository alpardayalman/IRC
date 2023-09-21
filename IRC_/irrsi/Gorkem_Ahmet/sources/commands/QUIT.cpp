# include "../../includes/Server.hpp"

/*
	// :syrk!kalt@millennium.stealth.net QUIT :Gone to have lunch
*/
void	Server::quit( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "QUIT" << END << std::endl;
	Server::serverInfo();
	Server::leaveAllChannel(it);
	std::cout << "Connection closed by client " << it->getHostname() << ":" << it->getPort() << std::endl;
	it->setIRCstatus(DISCONNECTED);
	it->sendMessageFd(RPL_QUIT(it->getPrefix(), tokenArr.at(0)));
	it->sendMessageFd(RPL_PONG(it->getPrefix(), it->getHostname()));

	// for (size_t i = 0; i < _pollfds.size(); i++)
	// {
	// 	if (it->getFd() == _pollfds[i].fd)
	// 	{
	// 		it->sendMessageFd(RPL_QUIT(it->getPrefix(), tokenArr[0]));
	// 		close(it->getFd());
	// 		Server::removeClient(it->getFd());
	// 		close(_pollfds[i].fd);
	// 		_pollfds.erase(_pollfds.begin() + i);
	// 		break;
	// 	}
	// }
	// Server::serverInfo();
}
