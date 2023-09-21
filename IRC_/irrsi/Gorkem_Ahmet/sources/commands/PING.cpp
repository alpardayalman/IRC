# include "../../includes/Server.hpp"

void	Server::ping( Client* it, std::vector<std::string> tokenArr ) // OK
{
	if (!tokenArr.at(1).compare(it->getNickname())
			|| !tokenArr[1].compare(this->_host))
		it->sendMessageFd(ERR_NEEDMOREPARAMS(it->getNickname(), "PING"));
	it->sendMessageFd(RPL_PING(it->getNickname(), tokenArr[1]));
}
