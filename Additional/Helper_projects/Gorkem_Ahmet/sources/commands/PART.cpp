# include "../../includes/Server.hpp"

/**
 * @brief 
 * 
 * Irssi -> IRC Server
 * Eger part komutu Irssi tarafindan ana dizindeyse;
 * 	/part 			-> 12:03 -!- Irssi: Not enough parameters given
 * 	/part asdf		-> Message:>PART-asdf<
 * Eger part komutunu Channel'in icerisinde yazarsak;
 * 	/part 			-> Message:>PART-#asdf<
 * 	/part See ya	-> Message:>PART-#asdf :See ya<
 * 
 * Response Example>:WiZ!jto@tolsun.oulu.fi PART #playzone :I lost<
 * 
 * // Message:>PART-#asdf :Lorem ipsum naber ehe.<
 * @param it 
 * @param tokenArr 
 */
void	Server::part( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "PART" << END << std::endl;

	if (it->getIRCstatus() != AUTHENTICATED && it->getIRCstatus() != DISCONNECTED) // DC neden var: irssi kendisi aktif değilken otomatik çıkıyor, yani part komutunu kullanıyor
	{
		it->sendMessageFd(RPL_NOTICE(it->getPrefix(), it->getNickname(), "Client's status is insufficient."));
		return ;
	}

	if (it->isRegisteredChannel(tokenArr.at(1)) == true) // Client tarafından belirlenen channel ismi var mı kontrol ediliyor, '/part #asdf'
	{
		Channel *chan = _channels[tokenArr[1]]; // Ayrılış gerçekleştirdiği channel'ın pointer'ı tutuluyor.
		std::string msg = "";
		if (tokenArr.size() > 2) // Client, Channel'den ayrılırken, Channel'e kullancının ayrıldığı bilgisi gönderildiği için ve bu mesajda ayrılış nedenide olduğundan dolayı bu kısımda 'ayrılma nedeni' kontrol edilmektedir.
			msg = Server::combineMessage(2, tokenArr); // Gelen mesajlar splitlendiğinden dolayı birleştirilmek zorundadır.
		// it->sendMessageFd(RPL_PART(it->getPrefix(), tokenArr[1], msg)); // Client'tın kanaldan ayrılması için eklenmiştir.
		chan->sendMessageBroadcast(RPL_PART(it->getPrefix(), tokenArr[1], msg)); // Client, kanaldan ayrılırken channel'daki herkese ayrılış mesajı göndermelidir 'BEN_ [~AHMET@176.88.10.20] has left #42kocaeli []'
		chan->removeClient(it); // Client, channel'dan ayrıldığından dolayı, channel'den o kullanıcı bilgisi kaldırılmalıdır.
		chan->removeAdmin(it); // Client, channel'dan ayrıldığı zaman adminlik yetkisi varsa kaybediyor.
		it->unregisterChannel(chan); // Client'ta bulunan channel bilgisinden channel kaldırılmalıdır.
		if (chan->getClientCount() == 0) // Eğerki channel'daki son kişi ise, channel silinmelidir.
			Server::removeChannel(chan->getName());
		else if (chan->getClientCount() == 1 && chan->searchClient(this->getClientByNickname("ircBot")) != NULL) // Channel'da 1 kişi kaldıysa ve bot ise silinmelidir.
		{
			Client *bot = Server::getClientByNickname("ircBot");
			bot->sendMessageFd(RPL_PART(bot->getPrefix(), tokenArr[1], std::string("I can't stand being alone!")));
			bot->unregisterChannel(chan);
			Server::removeChannel(chan->getName());
		}
		else if (Server::isChannelAdmin(it, chan) == true) // Eğerki channel'dan ayrılan kişi admin ise,
		{
		 	chan->addAdmin(chan->_channelClients[0]); // Adminlik channele kayıtlı olan bir sonraki kişiye verilir.
			chan->sendMessageBroadcast(RPL_MODE(it->getPrefix(), tokenArr[1], "+o", chan->_channelClients[0]->getNickname()));
		}
	}
	else // Eğer o isme sahip bir channel yoksa hata döndürülüyor.
		it->sendMessageFd(ERR_NOSUCHCHANNEL(it->getPrefix(), tokenArr[1]));
}
