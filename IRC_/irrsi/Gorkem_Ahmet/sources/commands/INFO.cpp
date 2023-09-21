# include "../../includes/Server.hpp"

void	Server::info( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "INFO" << END << std::endl;
	(void)tokenArr;
	it->sendMessageFd(RPL_INFO(it->getPrefix(), std::string("This ft_irc project was created by akaraca and gsever.")));
	it->sendMessageFd(RPL_INFO(it->getPrefix(), std::string("Server's Users:")));
	for (itClients itC = _clients.begin(); itC != _clients.end(); ++itC)
	{
		Client *dest = itC->second;
		std::stringstream ss;
		ss << dest->getPort();
		it->sendMessageFd(RPL_WHOISUSER(it->getPrefix(), dest->getNickname(), dest->getUsername(), dest->getHostname(), ss.str(), dest->getRealname())); // Aranan kullanıcı hakkında bilgiyi yazdırır
		// it->sendMessageFd(RPL_WHOISSERVER(it->getPrefix(), this->_serverName));
		if (dest->getRegisteredChannels().size() != 0)
			it->sendMessageFd(RPL_WHOISCHANNELS(it->getPrefix(), dest->getNickname(), Server::getRegisteredChannels(dest)));
	}
	it->sendMessageFd(RPL_ENDOFWHOIS(it->getPrefix(), ""));

	it->sendMessageFd(RPL_ENDOFINFO(it->getPrefix()));
}
