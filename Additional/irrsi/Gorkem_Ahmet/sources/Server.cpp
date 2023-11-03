/**
 * @file Server.cpp
 * @author Görkem SEVER (gsever), Ahmet KARACA (akaraca)
 * @brief 
 * @version 0.1
 * @date 2023-08-27
 * 
 * @note: How messaging 2 client each other?
 * @link http://chi.cs.uchicago.edu/chirc/irc_examples.html
 * 
 * @note: All IRC commands.
 * @link http://www.ae.metu.edu.tr/~evren/history/documents/commands.htm
 * @link http://www.csun.edu/~webteach/mirc/commands.html
 * @link https://www.antionline.com/showthread.php?136933-IRC-flags
 * @link https://modern.ircdocs.horse/#irc-concepts
 * 
 * @copyright Copyright (c) 2023
 * 
 */
# include "../includes/Server.hpp"

/**
 * @brief Construct a new Server:: Server object.
 * 
 * Creating Server and inits...
 * 
 * @fn Server::check(); Checking parameters. If parameters are okay return 'true'.
 * @fn Server::port(); Checking parameters. If port argument is within
 *  'unsigned short' range, returning port with unsigned short type.
 * @fn Server::pasword(); Checking parameters. If password is okay,
 *  returning password string.
 * @fn Server::openSocket(); Creating(opening) a socket for Server.
 * @fn Server::createSocketAddress(); Creating and seting
 *  socket's address types. For example; Address Family...
 * @fn Server::startListening(); Adding waiting list for connections...
 * @fn Server::initCommands(); Initializing all IRC commands.
 * 
 * @param argc 
 * @param argv 
 */
Server::Server( int argc, char **argv )
	:	_isCheck(check( argc )),
		_port(port( argv[1] )),
		_password(password( argv[2] )),
		_host( "127.0.0.1" ),
		_isRun( true ),
		_serverName( "ft_irc")
{
	std::cout << "Server Constructor called." << std::endl;
	openSocket();
	createSocketAddress();
	startListening();
	initCommands();
	std::cout << GREEN "Socket succesfully configured." END << std::endl;
}

/**
 * @brief Destroy the Server:: Server object
 * 
 * Deleting and terminating Server.
 * 
 * @fn std::map::clear(); Deleting container array.
 * @fn close(); Closing opened fd.
 */
Server::~Server( void )
{
	t_cmdFunc.clear();
	// _channels'ı silme
	for (itChannels it = _channels.begin(); it != _channels.end(); ++it)
		delete it->second;
	_channels.clear();
	// _clients'ı silme
	for (itClients it = _clients.begin(); it != _clients.end(); ++it)
		delete it->second;
	_clients.clear(); // Map'i temizle
	for (size_t i = 0; i < _pollfds.size(); ++i) {
		close(_pollfds[i].fd); // Kapatılan dosyaları serbest bırakmak (isteğe bağlı)
	}
	_pollfds.clear();
	close(this->_serverFd); // Closing the server.
	std::cout << "Server succesfully closed!" << std::endl;
}

/**
 * @brief Server started. Main loop here.
 * 
 * TR:
 * Events değişkeni, poll() fonksiyonuna gönderilen soketlerin
 *  beklediği olayları temsil eder.
 * Revents değişkeni, poll() fonksiyonunun döndürdüğü soketlerin
 *  gerçekleşen olayları temsil eder. 
 * poll() fonksiyonu, soketlerden gelen olayları beklemek için kullanılır.
 *  Fonksiyon, beklediği olayların gerçekleşmesi için bir süre bekler.
 *  Eğer beklediği olaylar gerçekleşmezse, fonksiyon 0 değeri döndürür.
 *  Bu durumda, revents değişkeni de 0 olacaktır.
 * 
 * EN:
 * The Events variable represents the events expected by sockets
 *  sent to the poll() function.
 * The Revents variable represents the events that occurred on the
 *  sockets returned by the poll() function.
 * The poll() function is used to wait for events from sockets.
 *  The function waits for a while for the events it expects to occur.
 *  If the expected events do not occur, the function returns 0.
 *  In this case, the revents variable will also be 0.
 * 
 * @fn Server::addToPollfds(); 
 */
void	Server::start( void )
{
	Server::addToPollfds( this->_serverFd, POLLIN, 0 );
	std::cout << "Server listening on port: " << this->_port << std::endl;
	std::cout << "Server password: " << this->_password << std::endl;
	while (this->_isRun)
	{
		if (poll(_pollfds.begin().base(), _pollfds.size(), -1) < 0)
			throw std::runtime_error("Error while polling from fd: " + std::string(strerror(errno)));
		for (itPoll it = _pollfds.begin(); it != _pollfds.end(); it++)
		{
			if (it->revents == 0)
				continue;

			if (it->revents & POLLHUP) // client'in bağlantısı koptuğunda otomatik olarak buraya girer lakin linuxta değil :/
			{
				Client *at = Server::getClientByFd(it->fd);
				if (at != NULL)
				{
					Server::leaveAllChannel(at);
					Server::removeClient(it->fd);
					_pollfds.erase(it);
					Server::serverInfo();
				}
				std::cout << "Client disconnected." << std::endl;
				break; // Move to the next socket
			}

			if (it->revents & POLLIN)
			{
				if (it->fd == _serverFd)
				{ 
					Server::acceptClients();
					break;
				}
				Client *at = Server::getClientByFd(it->fd);
				if (at != NULL)
				{
					if (at->getIRCstatus() == DISCONNECTED) // linuxta quit komutuna girdikten sonra pollfd POLLHUP olmuyor veya client'le bağlantısı kopunca olmuyor, halen POLLIN durumunda oluyor mecburen koydum.
					{
						Server::leaveAllChannel(at);
						Server::removeClient(it->fd);
						std::cout << "Client disconnected." << std::endl;
						_pollfds.erase(it);
						Server::serverInfo();
						break;
					}
					else
						Server::commandHandler(at);
				}
				else
					_pollfds.erase(it);
			}
		}
	}
}

void	Server::acceptClients( void )
{
	sockaddr_in	clientAddress = {};
	socklen_t	clientAddressSize = sizeof(clientAddress);

	int	clientFd = accept(this->_serverFd, (sockaddr *) &clientAddress, &clientAddressSize);
	if (clientFd < 0)
		std::cerr << "Error while accepting client connection: " << strerror(errno) << std::endl;
	else if (clientFd == 0)
		std::cerr << "Unexpected return value from accept(): 0" << std::endl; // Bu durum normal bir durum değildir ve genellikle hata işleme mantığı içinde değerlendirilmez.
	else
	{
		unsigned short clientPort = ntohs(clientAddress.sin_port);
		std::string clientHostname = inet_ntoa(clientAddress.sin_addr);
		if (clientPort != 0 && !clientHostname.empty())
		{
			Client	*client = new Client(clientFd, clientPort, clientHostname);
			if (client)
			{
				this->_clients.insert(std::make_pair(clientFd, client));
				std::cout << "New Client connected." << std::endl;
				std::cout << "Client's hostname: " << clientHostname << std::endl;
				std::cout << "Client's port: " << clientPort << std::endl;
				std::cout << "Client's fd: " << clientFd << std::endl;
				Server::addToPollfds(clientFd, POLLIN, 0);
				return ;
			}
			else
				std::cerr << "Error creating Client object: " << strerror(errno) << std::endl;
		}
		else
			std::cerr << "Client information is incomplete. Not creating Client object." << std::endl;
	}
	close(clientFd);
}

/*
// std::vector<std::string> tokenArr = splitMessage(buffer);
// MODE gsever akaraca gorkem ahmet
*/
void	Server::commandHandler( Client* at )
{
	char buffer[MAX_BUFFER];

	ssize_t bytesRead = recv(at->getFd(), buffer, sizeof(buffer) - 1, 0);
	if (bytesRead > 0)
	{
		buffer[bytesRead] = '\0';
		std::map<std::string, std::string> tokens = splitMessage("\r\n", buffer);
		for(itSplit itToken = tokens.begin(); itToken != tokens.end(); ++itToken)
		{
			std::cout << BLUE << "Message:>" << itToken->first << "-" << itToken->second << "<" << END << std::endl;
			for (itCmdFunc itCF = t_cmdFunc.begin(); itCF != t_cmdFunc.end(); ++itCF)
			{
				if (itToken->first.compare(itCF->first) == 0)
				{
					try
					{
						if (at->getIRCstatus() != DISCONNECTED)
						{
							std::vector<std::string> cmd = cmdMessage(itToken->second);
							cmd.insert(cmd.begin(), itToken->first);
							(this->*(itCF->second))(at, cmd);
							cmd.clear();
						}
						break;
					}
					catch (std::out_of_range& e) // std::vector::operator[]() tarzında doğrudan index'e erişirken gelen parametre sayısını kontrol etmek zorunda kalıyoruz, bunun yerine at() kullanarak, hatalı argüman girişi varsa trycatch bizim yerimize yakalıyor.
					{
						at->sendMessageFd(ERR_NEEDMOREPARAMS(at->getPrefix(), itToken->first));
					}
				}
			}
		}
		tokens.clear();
	}
	else
	{
		at->setIRCstatus(DISCONNECTED);
		// bytesRead <= 0 durumunda POLLHUP otomatik olarak tetiklenir, bu nedenle burada ek bir işlem yapmanıza gerek yok
	}
}

void	Server::removeClient(int clientFd)
{
	itClients it = _clients.find(clientFd);
	if (it != _clients.end())
	{
		close(it->second->getFd());
		delete it->second; // Belleği serbest bırak
		_clients.erase(it);
	}
}

void	Server::removeChannel( std::string channelName )
{
	itChannels it = this->_channels.find(channelName);
	if (it != _channels.end())
	{
		std::cout << it->second->getName() << ": Channel removed." << std::endl;
		delete it->second;
		_channels.erase(it);
	}
}

void	Server::quitReason( Client* client, std::string message )
{
	std::vector<std::string> msg;
	msg.push_back(message);
	Server::quit(client, msg);
}

void	Server::serverInfo( void )
{
	std::cout << BLUE << "------------POLL.FDS-----------------" << END << std::endl;
	for (size_t i = 0; i < this->_pollfds.size(); i++)
		std::cout << this->_pollfds[i].fd << " - ";
	std::cout << BLUE << "\n------------CLIENTS.FDS--------------" << END << std::endl;
	for (itClients it = _clients.begin(); it != _clients.end(); it++)
		std::cout << it->first << " - ";
	std::cout << BLUE << "\n------------CHANNELS-----------------" << END << std::endl;
	for (itChannels it = _channels.begin(); it != _channels.end(); it++)
		std::cout << it->first << " - ";
	std::cout << BLUE << "\n-------------------------------------" << END << std::endl;
}
