# include "../../includes/Server.hpp"

/*
	// `NICK``yuandre``USER``yuandre``yuandre``localhost``:Görkem``Sever`
*/
void	Server::nick( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "NICK" << END << std::endl;

	if (it->getIRCstatus() == HANDSHAKE)
	{
		if (tokenArr.size() == 2)
			it->setNickname(tokenArr[1]);
		else
			Server::quitReason(it, "Erroneous nickname");
		return ;
	}

	if (it->getIRCstatus() != AUTHENTICATED && it->getIRCstatus() != RENICK)
	{
		it->sendMessageFd(RPL_NOTICE(it->getPrefix(), it->getNickname(), "Client's status is insufficient."));
		return;
	}

	std::string name = tokenArr.at(1);
	for (size_t i = 0; i < name.size(); ++i)
	{
		if (!isprint(name[i]))
		{
			it->sendMessageFd(ERR_ERRONEUSNICKNAME(it->getPrefix(), name));
			return ;
		}
	}

	if (name[0] == '#' || name.size() > 16) // saçma bir nick girme işlemi gerçekleşirse önlemek için. // kullanıcı adının başında # olamaz.
		it->sendMessageFd(ERR_ERRONEUSNICKNAME(it->getPrefix(), name)); // Client'e uygun olmayan kullanıcı adına sahip olduğu bilgisini gönderiyoruz. Kayıtlı kullanıcıları ise sadece uyarmak için vardır.
	else if (Server::getClientFdByNickname(name) != -1 && Server::getClientFdByNickname(name) != it->getFd()) // Aynı isme sahip bir client yoksa -1 döndürür.
		it->sendMessageFd(ERR_NICKNAMEINUSE(name)); // Client, server'a kayıtlı aynı nick'e sahip bir kullanıcı varsa, bu ismi alamayacağını belirtmek için vardır.
	else
	{
		if (it->getIRCstatus() == AUTHENTICATED) // Sadece kayıtlı kullancıların erişebileceği, nick değiştirme yeridir.
		{
			it->sendMessageFd(RPL_NICK(it->getPrefix(), name)); // Client'e kullanıcı adının değiştiği bilgisi verilir.
			for (size_t i = 0; i < it->getRegisteredChannels().size(); ++i)
				it->getRegisteredChannels()[i]->sendMessageBroadcast(it, RPL_NICK(it->getPrefix(), name)); // Bağlı olduğu tüm channelere isminin değiştiği bilgisi gidecek.
			it->setNickname(name); // RPL_NICK kısmında eski kullanıcı adına ihtiyaç olduğundan dolayı (hangi kullanıcın adı, ne oldu?) en sona client'in nick'ini belirleme eklendi.
		}
		else if (it->getIRCstatus() == RENICK)// ilk bağlantıdaki client'ın adı aynı ise buraya girer.
		{
			std::stringstream ss;
			ss << it->getPort();
			it->setNickname(name);
			it->setIRCstatus(AUTHENTICATED); // Ilk bağlantı ile güvenli bir şekilde nick belirlendiyse kullanıcının kimlik doğrulaması ok sayılır.
			if (it->getNickname().compare("ircBot") != 0)
				it->sendWelcomeMessage(Server::welcomeServer()); // ilk bağlantı olduğundan dolayı, emoji mesajıdır
			it->sendMessageFd(RPL_WELCOME(name, _serverName));  // ilk bağlantı olduğundan dolayı, selamlama mesajıdır
			it->sendMessageFd(RPL_NOTICE(it->getPrefix() + ":" + ss.str() , it->getNickname(), "Client status is now: AUTHENTICATED."));
			it->sendMessageFd(RPL_NICK(it->getPrefix(), name)); // Client'e kullanıcı adının değiştiği bilgisi verilir.
			it->sendMessageFd(RPL_MODE(it->getPrefix(), it->getNickname(), it->getClientMods(), ""));
		}
	}
}
