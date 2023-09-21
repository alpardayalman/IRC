# include "../../includes/Server.hpp"

/*
	"/list -y" yazınca tüm kanalları listeleyecek.
 
	#42kocaeli
	#BEn
	#VoidChannel
	...

*/
void	Server::list( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "LIST" << END << std::endl;
	if (it->getIRCstatus() != AUTHENTICATED)
	{
		it->sendMessageFd(RPL_NOTICE(it->getPrefix(), it->getNickname(), "Client's status is insufficient."));
		return ;
	}
	it->sendMessageFd(RPL_LISTSTART(it->getPrefix()));

	size_t i = tokenArr.size();
	std::stringstream ss;
	itChannels itChan;
	if (i == 1) // '/list -y' yani tüm channelleri listelemek için
	{
		for (itChan = this->_channels.begin(); itChan != this->_channels.end(); ++itChan)
		{
			ss.str("");
			ss << itChan->second->getClientCount();
			it->sendMessageFd(RPL_LIST(it->getPrefix(), itChan->second->getName(), ss.str(), itChan->second->getChannelTopic()));
		}
	}
	else // '/list a #b c d #x x f' yani belirli channelleri listelemek için
	{
		for (size_t l = 1; l < i; ++l)
		{
			ss.str("");
			if (tokenArr[l][0] != '#')
				tokenArr[l] = "#" + tokenArr[l];
			itChan = _channels.find(tokenArr[l]);
			if (itChan != _channels.end())
			{
				ss << itChan->second->getClientCount();
				it->sendMessageFd(RPL_LIST(it->getPrefix(), itChan->second->getName(), ss.str(), itChan->second->getChannelTopic()));
			}
			else
				it->sendMessageFd(ERR_NOSUCHCHANNEL(it->getPrefix(), tokenArr[l]));
		}
	}
	it->sendMessageFd(RPL_LISTEND(it->getPrefix()));
}
