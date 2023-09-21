# include "../../includes/Server.hpp"

/*
	// `PASS``asdf``NICK``yuandre``USER``yuandre``yuandre``localhost``:Görkem``Sever`
*/
void	Server::pass( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "PASS" << END << std::endl;

	if (it->getIRCstatus() != HANDSHAKE)
	{
		it->sendMessageFd(RPL_NOTICE(it->getPrefix(), it->getNickname(), "Client's status is insufficient."));
		return ;
	}

	if (!tokenArr.empty() && !tokenArr.at(0).compare("PASS"))
		if (!tokenArr.at(1).compare(this->_password))
			it->setPasswordStatus();
		else
			Server::quitReason(it, "Password is wrong!");
	else
		Server::quitReason(it, "Password is empty!");
}
