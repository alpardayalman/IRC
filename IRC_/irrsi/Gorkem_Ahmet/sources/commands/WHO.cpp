# include "../../includes/Server.hpp"

/*
	//Channel içindeyken /who yazınca bu çıktıyı veriyor, ana sayfaya.

	"/who asdf" yazarsan, asdf'nin başında # olmadığı içni ele alma.
	"/who #asdf" ise, asdf channelini ara ve kullancıların bilgilerini bastır.
	"/who #A #B #C" şeklindeki bir yapıda sadece #A ele alınacaktır.
	17:13 -!- #42kocaeli akaraca   H   0  
						~akaraca@2d6b-1b29-411f-5a9-8db6.88.176.ip 
						[k2m15s08.42kocaeli.com.tr]
	17:13 -!- #42kocaeli gsever    H@  4  
						~gsever@2d6b-1b29-411f-5a9-8db6.88.176.ip [Gorkem 
						Sever]
	17:13 -!- End of /WHO list

	--> ERR_NOSUCHCHANNEL
	--> RPL_WHOREPLY
	--> RPL_ENDOFWHO

*/
void	Server::who( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "WHO" << END << std::endl;

	if (it->getIRCstatus() != AUTHENTICATED)
	{
		it->sendMessageFd(RPL_NOTICE(it->getPrefix(), it->getNickname(), "Client's status is insufficient."));
		return ;
	}

	if (tokenArr.at(1)[0] == '#') //kanal aramak için
	{
		itChannels itC = _channels.find(tokenArr[1]); // channeli bul.
		if (itC != _channels.end()) // eğer channel varsa
		{
			Channel* itChanel = itC->second;
			for (size_t i = 0; i < itChanel->_channelClients.size(); i++)
			{
				Client* client = itChanel->_channelClients[i];
				std::string isAdmin = "";
				if (itChanel->searchAdmin(client) != NULL)
					isAdmin = "@";
				it->sendMessageFd(RPL_WHOREPLY(it->getPrefix(), tokenArr[1], "~" + client->getUsername(), client->getHostname(), _serverName, client->getNickname(), isAdmin, "0", client->getRealname()));
				// @ -> kullanıcının operator olduğunu temsil ediyor.
				// "0" ise kullanıcının o anki sunucu üzerinden mesajlaştığını ifade eder.

				//  * abc       H   3
				// "H 3" ifadesi, kullanıcının "abc" takma adına sahip olduğunu ve sunucu ile arasında 3 "hop" olduğunu belirtir. Yani, bu kullanıcı ile mesajlar üç aracı sunucu aracılığıyla iletilmiştir.
			}
			it->sendMessageFd(RPL_ENDOFWHO(it->getPrefix(), tokenArr[1]));
		}
		else // channel yoksa hata döndür.
			it->sendMessageFd(ERR_NOSUCHCHANNEL(it->getPrefix(), tokenArr[1])); // #x: No such channel
	}
	else // Burada sadece serverda arama yaptığımız için admin işaretine gerek yok??
	{
		//	Server'da kullanıcıyı aramak için yapılıyor.
		//	Channel bilgisi "*" şeklinde gözüküyor.
		//	Aradağı kişi yoksa herhangi bir çıktı vermiyor sadece End of /WHO list çıktısı bulunuyor.
			// 09:56 -!-          * akaraca   H   3  ~akaraca@bbdb-4a67-88a1-bfa9-1d6d.190.78.ip [ahmet karaca]
			// 09:56 -!- End of /WHO list
		// "/who akaraca X Ben" şeklinde bir girdi olsada sadece akaraca hakkında bilgi veriliyor.
		int fd = Server::getClientFdByNickname(tokenArr[1]);
		if (fd != -1)
		{
			Client *client = _clients.at(fd);
			it->sendMessageFd(RPL_WHOREPLY(it->getPrefix(), "*", "~" + client->getUsername(), client->getHostname(), _serverName, client->getNickname(), "","0", client->getRealname()));
		}
		it->sendMessageFd(RPL_ENDOFWHO(it->getPrefix(), "*"));
	}
}
