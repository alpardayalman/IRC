#include "Bot.hpp"

void	Bot::openSocket( void )
{
	// <sys/socket.h>
	// AF_INET: Address Family IPv4. Internetwork: UDP, TCP, etc.
	// SOCK_STREAM: For TCP.
	// '0': Automatically choose the appropriate protocol.
	this->_botFd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_botFd < 0)
		throw std::runtime_error("Error while opening socket: " + std::string(strerror(errno)));
	std::cout << YELLOW "Socket opened." END << std::endl;
}

void	Bot::connectSocketAddress( void )
{
	struct addrinfo hints, *result;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET; // IPv4
	hints.ai_socktype = SOCK_STREAM; // TCP

	std::stringstream ss;
	ss << this->_port;

	int status = getaddrinfo(this->_host.c_str(), ss.str().c_str(), &hints, &result);
	if (status != 0)
	{
		close(this->_botFd);
		throw std::runtime_error("Error while getting address information: " + std::string(strerror(errno)));
	}

	// struct sockaddr_in serverAddress;

	// // Setting 0 &serverAddress's sizeof() bytes.
	// memset((char *) &serverAddress, 0, sizeof(serverAddress));
	// std::cout << YELLOW "Socket address created." END << std::endl;

	// serverAddress.sin_family = AF_INET;
	// serverAddress.sin_addr = ((struct sockaddr_in *)result->ai_addr)->sin_addr;///inet_addr(inet_ntoa(*(struct in_addr *)serverInfo->h_addr_list[0]));//inet_addr(_host.c_str()); // şeklinde bağlantı kuramayız çünkü host değeri "localhost"'tur yani string'tir 
	// serverAddress.sin_port = ((struct sockaddr_in *)result->ai_addr)->sin_port;//htons(this->_port);
	// freeaddrinfo(result);
	// std::cout << YELLOW "Socket address settings set." END << std::endl;

	// if (connect(this->_botFd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
	if (connect(this->_botFd, result->ai_addr, result->ai_addrlen) < 0)
	{
		close(this->_botFd);
		throw std::runtime_error("Error while connecting to host: " + std::string(strerror(errno)));
	}
	std::cout << YELLOW << "Connected to " << this->_host << " on port " << this->_port << END <<  std::endl;
}
