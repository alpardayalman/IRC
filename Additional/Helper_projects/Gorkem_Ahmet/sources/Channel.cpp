# include "../includes/Channel.hpp"

Channel::Channel( std::string name, std::string password )
	: _name(name), _clientCount(0), _channelModes(""), _k(password), _l(0)
{
	std::cout << "Channel Created: Name: " << this->getName() << std::endl;
}

Channel::~Channel( void )
{
	std::cout << "Channel Deleting: Name: " << this->getName() << std::endl;
}

Client*	Channel::getAdmin( std::string nickname )
{
	for (size_t i = 0; i < this->_admins.size(); i++)
	{
		if (this->_admins[i]->getNickname().compare(nickname) == 0)
			return (this->_admins[i]);
	}
	return (NULL);
}

void	Channel::addClient( Client* client )
{
	this->_channelClients.push_back(client);
	this->_clientCount++;
	std::cout << this->getName() << ": " << client->getNickname()
		<< " join the channel." << std::endl;
}

void	Channel::addAdmin( Client* admin )
{
	if (admin == NULL)
		return ;
	std::cout << this->getName() << ": " << admin->getNickname()
		 << " was given the position of admin." << std::endl;
	this->_admins.push_back(admin);
}

void	Channel::removeClient( Client* client )
{
	itChannelClients it = std::find(_channelClients.begin(),
		_channelClients.end(), client);
	if (it != _channelClients.end())
	{
		_channelClients.erase(it);
		this->_clientCount--;
		std::cout << this->getName() << ": " << client->getNickname()
			<< " left the channel." << std::endl;
	}
}

void	Channel::removeAdmin( Client* admin )
{
	itChannelAdmins itCA = std::find(this->_admins.begin(),
		this->_admins.end(), admin);
	if (itCA != this->_admins.end())
	{
		this->_admins.erase(itCA);
		std::cout << this->getName() << ": " << admin->getNickname()
			<< "'s admin status has been removed." << std::endl;
	}
}

void	Channel::sendMessageBroadcast( std::string message )
{
	for	(itChannelClients itCli = this->_channelClients.begin();
			itCli != this->_channelClients.end(); itCli++)
	{
		(*itCli)->sendMessageFd(message);
	}
}

/**
 * @brief Eger bir Client Channel'e mesaj attiginda burasi calisacak.
 * 
 * Cunku yazdigi mesaji kanaldaki herkese gonderip, kendisine gondermemesi lazim.
 * 
 * @param exclude Mesaji gonderen Client'in kendisi.
 * @param message 
 */
void	Channel::sendMessageBroadcast( Client* exclude, std::string message )
{
	for	(itChannelClients itCli = this->_channelClients.begin(); itCli != this->_channelClients.end(); itCli++)
	{
		if (*itCli == exclude)
			continue;
		(*itCli)->sendMessageFd(message);
	}
}

void	Channel::channelUsers(Client* client, Channel* channel, std::string channelName )
{
	for (size_t i = 0; i <  channel->_channelClients.size(); ++i)
	{
		std::string authority = "";
		// if (channel->_channelClients[i]->getNickname().compare(channel->getAdmin()->getNickname()) == 0)
		if (channel->searchAdmin(channel->_channelClients[i]) != NULL)
			authority = "@";
		client->sendMessageFd(RPL_NAMREPLY(client->getNickname(), channelName, authority + channel->_channelClients[i]->getNickname()));
	}
	client->sendMessageFd(RPL_ENDOFNAMES(client->getPrefix(), channelName));
}

/**
 * @brief Disaridan verdigimiz Client, Channel'in Admin'leri arasinda var mi?
 * 
 * @param client 
 * @return Client* :Eger varsa Channel'deki Admin'i, yoksa NULL dondur.
 */
Client*	Channel::searchAdmin( Client* client )
{
	if (!client)
		return (NULL);
	for (size_t i = 0; i < this->_admins.size(); i++) // Adminler arasinda disaridan verilen Client araniyor.
	{
		if (!client->getNickname().compare(this->_admins[i]->getNickname()))
			return (this->_admins[i]);
	}
	return (NULL);
}


/**
 * @brief Disaridan verdigimiz Client, Channel'in Client'leri arasinda var mi?
 * 
 * @param client 
 * @return Client* :Eger varsa Channel'deki Client'i, yoksa NULL dondur.
 */
Client*	Channel::searchClient( Client* client )
{
	if (!client)
		return (NULL);
	for (size_t i = 0; i < this->_channelClients.size(); i++) // Adminler arasinda disaridan verilen Client araniyor.
	{
		if (!client->getNickname().compare(this->_channelClients[i]->getNickname()))
			return (this->_channelClients[i]);
	}
	return (NULL);
}

void	Channel::setChannelMods( std::string mod )
{
	if (!mod.empty())
	{
		if (mod[0] == '-')
		{
			for (size_t i = 1; i < mod.size(); ++i)
			{
				if (_channelModes.find(mod[i]) != std::string::npos)
					_channelModes.erase(_channelModes.find(mod[i]), 1);
			}
		}
		else
		{
			if (mod[0] == '+')
				mod.erase(0, 1);
			for (size_t i = 0; i < mod.size(); ++i)
			{
				if (_channelModes.find(mod[i]) == std::string::npos)
					_channelModes += mod[i];
			}
		}
	}

	std::sort(_channelModes.begin(), _channelModes.end());
}
