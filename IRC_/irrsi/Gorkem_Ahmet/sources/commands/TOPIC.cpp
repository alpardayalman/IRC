# include "../../includes/Server.hpp"

/*
	/topic #channel :Damnn, moon is shining
*/
void	Server::topic( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "TOPIC" << END << std::endl;
	if (it->getIRCstatus() != AUTHENTICATED)
	{
		it->sendMessageFd(RPL_NOTICE(it->getPrefix(), it->getNickname(), "Client's status is insufficient."));
		return ;
	}

	if (tokenArr.at(1)[0] != '#')
		tokenArr[1] = "#" + tokenArr[1];

	itChannels itChannel = _channels.find(tokenArr[1]);
	if (itChannel != _channels.end())
	{
		if (Server::isChannelAdmin(it, itChannel->second))
		{
			if (tokenArr.at(2)[0] == ':')
				tokenArr[2].erase(0, 1);
			itChannel->second->sendMessageBroadcast(RPL_TOPIC(it->getPrefix(), tokenArr[1], Server::combineMessage(2, tokenArr)));
		}
		else
			it->sendMessageFd(ERR_CHANOPRIVSNEEDED(it->getPrefix(), tokenArr[1]));
	}
	else
		it->sendMessageFd(ERR_NOSUCHCHANNEL(it->getPrefix(), tokenArr[1])); // Channel yoksa hata döndürülür.
}
