#include "../includes/Server.hpp"

/**
 * @brief Opening a socket for Server.
 * 
 * @fn socket(); Socket for 
 * 	LIBRARY: <sys/socket.h>
 * 	PARAM: AF_INET: Address Family IPv4. Internetwork: UDP, TCP, etc.
 * 	PARAM: SOCK_STREAM: For TCP.
 * 	PARAM: '0': Automatically choose the appropriate protocol.
 * @fn strerror(); Returns a pointer to an error message string.
 *  It searches an internal array for the error number errnum and
 *  returns a pointer to the corresponding error message string.
 */
void	Server::openSocket( void )
{
	this->_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_serverFd < 0)
		throw std::runtime_error("Error while opening socket: " + std::string(strerror(errno)));
	std::cout << YELLOW "Socket opened." END << std::endl;
}

/**
 * @brief Creating socket's address-family.
 * 
 * TR:
 * Bind(), bir soketi belirtilen yerel adrese bağlamak için kullanılır.
 * Listen(), bir soketin bağlantıları kabul etmeye hazır olduğunu
 *  belirtmek için kullanılır.
 * 
 * EN:
 * Bind() is used to bind a socket to the specified local address.
 * 
 * PARAM: INADDR_ANY: Bind all interfaces.
 * If you wish to bind your socket to localhost only, the syntax would be
 *  my_sockaddress.sin_addr.s_addr = inet_addr("127.0.0.1");.
 * 
 * @fn htons(): Converts the unsigned short integer this->_port
 *  from host byte order to network byte order.
 * @fn memset(); Allocating memory for serverAddress.
 * 	Setting 0 &serverAddress's sizeof() bytes.
 * @fn bind(); Assigns &serverAddress to sockfd address.
 * @fn close(); Closing server's fd.
 * @fn strerror(); Returns a pointer to an error message string.
 *  It searches an internal array for the error number errnum and
 *  returns a pointer to the corresponding error message string.
 */
void	Server::createSocketAddress( void )
{
	struct sockaddr_in serverAddress;

	memset((char *) &serverAddress, 0, sizeof(serverAddress));
	std::cout << YELLOW "Socket address created." END << std::endl;

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(this->_port);
	std::cout << YELLOW "Socket address settings set." END << std::endl;

	if (bind(this->_serverFd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
	{
		close(this->_serverFd);
		throw std::runtime_error("Error while binding socket: " + std::string(strerror(errno)));
	}
	std::cout << YELLOW "Socket address bound." END << std::endl;
}

/**
 * @brief Joining queue limit.
 * 
 * EN:
 * Listen() is used to indicate that a socket is ready to accept connections.
 * 
 * TR:
 * Listen(), bir soketin bağlantıları kabul etmeye hazır olduğunu
 *  belirtmek için kullanılır.
 * 
 * @fn listen(); 
 * 	PARAM: Accepting incoming connection request;
 * 	PARAM: MAX_CONNECTIONS, maximum length of the queue of pending connections.
 * @fn close(); Closing server's fd.
 * @fn strerror(); Returns a pointer to an error message string.
 *  It searches an internal array for the error number errnum and
 *  returns a pointer to the corresponding error message string.
 * 
 */
void	Server::startListening( void )
{
	if (listen(this->_serverFd, MAX_CONNECTIONS) < 0)
	{
		close(this->_serverFd);
		throw std::runtime_error("Error while listening on socket: " + std::string(strerror(errno)));
	}
	std::cout << YELLOW "Socket listening with a pending queue." END << std::endl;
}
