#include "../includes/Server.hpp"

bool	Server::check( int argc )
{
	std::cout << YELLOW "Checking parameters..." END << std::endl;
	if (argc != 3)
		throw( std::invalid_argument(RED "Error: Invalid Argument Count\n"
		"Usage: './ircserv <port> <password>'" END) );
	return (true);
}

unsigned short	Server::port( std::string port )
{
	// for: ' $> ./ircserv "" 1234 '
	//		' $> ./ircserv '' 1234 '
	if (port.empty())
		throw( std::invalid_argument(RED "Error: Invalid Port\n"
		"Port cannot be empty." END) );

	for (size_t i = 0; i < port.length(); ++i){
		if (!isdigit(port[i])){
			throw( std::invalid_argument(RED "Error: Invalid Port\n"
			"Port must be a integer." END) );
		}
	}

	unsigned short portVal = atoi(port.c_str());

	// Port numaraları, özel ayrılmış bazı portlar dışında genellikle 0 ile 65535 arasında değer alır.
	//	Ancak, 0 ile 1023 arasındaki port numaraları "well-known ports" olarak adlandırılır ve genellikle sistem servisleri veya tanınmış protokoller tarafından kullanılır.
	//	Bu port numaralarının birçoğu belirli bir protokol veya servis tarafından rezerve edilmiştir.
	//	Bu nedenle, kullanıcı tarafından belirlenecek port numaralarının genellikle 1024'den başlaması tercih edilir. 
	if (portVal < 1024 || portVal > std::numeric_limits<unsigned short>::max())
		throw( std::invalid_argument(RED "Error: Invalid Port\n"
		"Port must be between 1024 and 65535." END) );
	return (portVal);
}

std::string	Server::password( std::string password )
{
	// for: ' $> ./ircserv 1234 "" '
	//		' $> ./ircserv 1234 '' '
	if (password.empty())
		throw( std::invalid_argument(RED "Error: Invalid Password\n"
		"Password cannot be empty." END) );
	std::cout << GREEN "Parameters okay." END << std::endl;
	return (password);
}

void	Server::initCommands( void )
{
	t_cmdFunc["CAP"] = &Server::cap;
	t_cmdFunc["JOIN"] = &Server::join;
	t_cmdFunc["NICK"] = &Server::nick;
	t_cmdFunc["PASS"] = &Server::pass;
	t_cmdFunc["QUIT"] = &Server::quit;
	t_cmdFunc["USER"] = &Server::user;
	t_cmdFunc["PRIVMSG"] = &Server::privmsg;
	t_cmdFunc["PING"] = &Server::ping;
	t_cmdFunc["PART"] = &Server::part;
	t_cmdFunc["MODE"] = &Server::mode;
	t_cmdFunc["LIST"] = &Server::list;
	t_cmdFunc["INFO"] = &Server::info;
	t_cmdFunc["WHO"] = &Server::who;
	t_cmdFunc["WHOIS"] = &Server::whois;
	t_cmdFunc["KICK"] = &Server::kick;
	t_cmdFunc["TOPIC"] = &Server::topic;
}

/**
 * @brief Adds a new pollfd object to the Server class.
 * 
 * @fn std::vector::push_back(); Adds a new pollfd in container array.
 * 
 * @param fd The file descriptor to be added.
 * @param events The events to be added.
 * @param revents The events that occurred.
 */
void	Server::addToPollfds( int fd,  short events, short revents )
{
	pollfd newPoll;

	newPoll.fd = fd;
	newPoll.events = events;
	newPoll.revents = revents;
	this->_pollfds.push_back(newPoll);
}

/**
 * @brief Splits a message into tokens based on a given delimiter(\\r\\n)
 *  and stores them in a std::map.
 * 
 * @fn std::string::find(); If find this delimeter; return first
 *  delimeter index. If not find this delimeter; return std::npos;
 * @fn std::string::substr(); Getting start location to length.
 *  Example -> "helloo" -> .substr(2, 3); -> "llo".
 * @fn std::string::size(); Returns the length of the string,
 *  in terms of bytes.
 * @fn std::string::erase(); Erases part of the string,
 *  reducing its length.
 * @fn std::toupper(); Converting a character to uppercase.
 * @fn std::string::insert(); Inserts additional characters into
 *  the string right before the character indicated by pos (or p).
 * @fn std::make_pair
 * 
 * @param delimeter The delimiter used to split the message.
 * @param message The message to be split.
 * @return std::map<std::string, std::string> A map containing the tokens as key-value pairs.
 */
std::map<std::string, std::string>\
	Server::splitMessage(std::string delimeter, std::string message )
{
	std::map<std::string, std::string> tokens;
	size_t pos = 0;

	// CAP LS\r\nNICK gsever\r\nUSER A B C D E F G asdf\r\n
	while ((pos = message.find(delimeter)) != std::string::npos)
	{
		int posFirst = message.find(' ');

		std::string firstWord = message.substr(0, posFirst);
		// If we write 'INFO' last index have '\n' so remove '\r' and '\n' characters from the first word.
		for (size_t i = 0; i < firstWord.size(); ++i)
		{
			if (firstWord[i] == '\r' || firstWord[i] == '\n')
			{
				firstWord.erase(i, 1);
				--i; // Decrement i since a character is erased.
			}
		}
		// Convert the first word (command) to uppercase.
		for (size_t i = 0; i < firstWord.size(); ++i)
			firstWord[i] = std::toupper(firstWord[i]);

		// Split the message into command and arguments
		// 1. Kısım: USER 2. Kısım: A B C D E F G asdf
		//posFirst + 1; CAP'ten sonra gelene boşluğun indexi
		//pos - posFirst - 1; "CAP LS\r\n" yapısında LS'i almak için LS'in uzunluğuna bulmaya ihtiyaç var, -1 ise \r'ı almak istemiyoruz.
		if (posFirst != -1)
			tokens.insert(std::make_pair(firstWord, message.substr(posFirst + 1, pos - posFirst - 1)));
		else
			tokens.insert(std::make_pair(firstWord, "")); // nc localhost için 'TEK' argüman için kopya durumu söz konusu oluyor, bunu engellemek için eklendi. 'Message:>B-b<'
		message.erase(0, pos + delimeter.length());
	}
	// Handle the case where the message comes without a command
	if (!message.empty()) // Parsing unkown command anda netcat.
	{
		if (message[message.size() - 1] == '\n')
			return (Server::splitMessage("\n", message));
		else
		{
			message.append("\n");
			return (Server::splitMessage("\n", message));
			//tokens.insert(std::make_pair("UNKNOWN", message));
		}
	}
	return (tokens);
}

std::vector<std::string>	Server::cmdMessage( std::string message )
{
	std::stringstream			ss(message);
	std::string					token;
	std::vector<std::string>	tokenArr;

	while (ss >> token)
		tokenArr.push_back(token);

	// Print tokens.
	std::cout << B_BLUE << "Tokens:>";
	for (size_t i = 0; i < tokenArr.size(); i++)
		std::cout << "`" << tokenArr[i] << "`";
	std::cout << "<" << END << std::endl;
	return (tokenArr);
}

int	Server::getClientFdByNickname( std::string name )
{
	for (itClients it = this->_clients.begin(); it != _clients.end(); it++)
	{
		if (it->second->getNickname().compare(name) == 0 && it->second->getNickname().size() == name.size())
			return (it->second->getFd());
	}
	return (-1);
}

Client*	Server::getClientByNickname( std::string name )
{
	for (itClients it = this->_clients.begin(); it != _clients.end(); it++)
	{
		if (it->second->getNickname().compare(name) == 0 && it->second->getNickname().size() == name.size())
			return (it->second);
	}
	return (NULL);
}

Client*	Server::getClientByFd( int fd )
{
	for (itClients it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		if (it->second->getFd() == fd)
			return (it->second);
	}
	return (NULL);
}

std::string	Server::getRegisteredChannels( Client* Client )
{
	std::string channelString = "";

	for (itChannels itC = this->_channels.begin(); itC != this->_channels.end(); ++itC)
	{
		if (this->isChannelAdmin(Client, itC->second))
			channelString += "@";
		if (this->isChannelUser(Client, itC->second))
			channelString += itC->second->getName();
		channelString += " ";
	}
	return (channelString);
}

std::string	Server::combineMessage( size_t i, std::vector<std::string> vectorMessage )
{
	std::string msg;
	size_t size = vectorMessage.size();
	for (; i < size; i++)
	{
		if (i + 1 < size)
			msg += vectorMessage[i] + " ";
		else
			msg += vectorMessage[i];
	}
	return (msg);
}

bool	Server::isChannelAdmin(Client* client, Channel* channel)
{
	if (channel->searchAdmin(client) == NULL)
		return (false);
	return (true);
}

bool	Server::isChannelUser(Client* client, Channel* channel)
{
	if (channel->searchClient(client) == NULL)
		return (false);
	return (true);
}

void	Server::leaveAllChannel( Client* client )
{
	size_t size = client->getRegisteredChannels().size();
	for (size_t i = size; i > 0; --i) // Quit ile ayrılan channel, kayıtlı olduğu channel'lardan ayrılmalıdır.
	{
		std::vector<std::string> leaveChannel;
		leaveChannel.push_back("PART");
		leaveChannel.push_back(client->getRegisteredChannels()[i - 1]->getName());
		leaveChannel.push_back(":QUIT");
		Server::part(client, leaveChannel);
	}
	client->clearRegisteredChannels();
}

std::string	Server::welcomeServer( void )
{
	std::string	msg;

	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡠⠖⠚⠉⠉⣇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣶⠟⠋⠒⢦⡀⠀⠻⣄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡼⢻⠃⠀\033[0;31m⣶⠆\033[m ⣷⠖⠒⠢⣌⠉⠉⠒⠒⠂⢤⣄⠀⠀⠀⠀⠀⠀⠀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡾⠁⠈⢧⠀⠀⠀⢸ \033[0;31m⣶⡄\033[m ⠀⢳⡀⡀⠀⢀⡼⠃⠀⠀⠀⠀⠀⠀⠀\n";
	msg += " ⠀⠀⠀⠀⢠⡶⣦⠀⠀⠀⠀⠀⠀⢸⠁⠀⠀⠀⠙⠒⣶⣾⡄  ⠀⠀⢨⡇⣷⣴⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
	msg += " ⠀⣴⢻⣻⠿⠗⠛⠛⠉⠉⠉⠓⠒⠦⢤⣀⠀⠀⠠⠴⢿⡄⠙⠦⣤⣤⠤⠊⠐⠁⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
	msg += " ⣼⣿⣿⡹⡄⣀⡤⠤⠤⠤⠤⢤⣀⡀⠀⠈⣿⣦⡀⠀⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
	msg += " ⠹⣿⠈⠻⣏⠀⠀⠀⠀⠀⠀⠀⠀⠉⠓⢤⣿⢙⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
	msg += " ⠀⠹⣆⠀⠀⠙⠲⠦⢤⣤⣀⣀⣀⣀⣠⣼⠛⣿⢹⡇⠀⠀⠀⠀⠀⡀⠀⠀⠀⠀⢸⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
	msg += " ⠀⠀⠙⢷⣄⠀⠀⠀⠀⠀⠀⠀⣴⡞⠓⠛⠀⠘⢿⣄⡀⠀⢀⣠⡾⠃⠀⠀⠀⠀⠈⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
	msg += " ⠀⠀⠀⠀⠉⠓⠦⣤⣀⣀⠀⠀⣨⣷⠀⠀⠀⠀⠀⠈⠉⠉⠉⠁⠀⠀⠀⠀⠀⠀⠀⢹⡄⠀⠀⠀⠀⠀⠀⠀⠀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⣿⠛⠿⣤⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠀⠀⠀⠀⠈⣷⠀⠀⠀⠀⠀⠀⠀⠀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⠀⠀⠉⠛⠦⣄⣀⣀⣀⣠⠴⠚⠁⠀⠀⠀⠀⠀⢹⡇⠀⠀⠀⠀⠀⠀⠀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⡶⣿⠀⠀⠀⠀⠀⠀⠀⠀⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢿⡄⠀⠀⠀⠀⠀⠀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢾⠀⢿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣴⠞⠛⠛⠷⠀⠀⠀⠀⠈⣿⠀⠀⠀⠀⠀⠀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣧⣸⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡃⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⡇⠀⠀⠀⠀⠀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⣾⢋⠉⠛⠛⢷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠻⢶⣄⡀⠀⠀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠻⣿⡇⠀⠀⠀⠙⣦⣀⠀⠀⠀⠀⢀⣤⠤⠼⠿⣦⡄⠀⠀⠀⠀⠀⣶⣦⠀⠀⠉⢻⣄⠀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠳⢤⣤⡶⢿⣿⣬⡓⠶⠤⣤⣾⣥⠆⠀⠀⠀⠀⠀⠀⠀⢀⣴⠏⢸⡇⠀⠀⠀⣿⡀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠑⢦⡉⠻⠟⠳⡾⠛⠛⠿⣤⣇⣀⣀⣀⣤⠴⠞⠋⣁⣤⠞⠁⠀⠀⢀⣿⠀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣇⠀⠀⠀⠀⠉⠛⠛⠛⠛⠛⠛⠋⠉⠀⠀⠀⠀⠀⣼⠏⠀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠻⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⡾⠋⠀⠀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠓⠦⣤⣄⣀⣀⣀⣀⣀⣤⣤⣤⡶⠾⠛⠉⠀⠀⠀⠀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠉⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
	return (msg);
}
