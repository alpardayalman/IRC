#ifndef	SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <cstdlib>
# include <stdint.h>
# include <limits>
# include <sys/socket.h>
# include <sys/types.h>
# include <arpa/inet.h>
# include <cstring>
# include <netinet/in.h>
# include <unistd.h>
# include <fcntl.h>
# include <map>
# include <vector>
# include <sys/poll.h>
# include <netdb.h>
# include <sstream>
# include <signal.h>
# include <fstream>
# include <sys/stat.h>

# include "colors.hpp"
# include "Client.hpp"
# include "Channel.hpp"

# define MAX_CONNECTIONS 10
# define MAX_BUFFER 1024

class Client;
class Channel;
class Server;

typedef std::vector<pollfd>::iterator					itPoll;
typedef std::map<std::string, std::string>::iterator	itSplit;
typedef std::map<int, Client *>::iterator				itClients;
typedef std::map<std::string, Channel *>::iterator		itChannels;

typedef void (Server::*CmdFunc)( Client*, std::vector<std::string> );
typedef std::map<std::string, CmdFunc>::iterator	itCmdFunc;

class Server
{
/* _________________________ VARIABLES ______________________________________ */
	private:
		bool								_isCheck; // arguments size is okay.
		const unsigned short				_port; // server port number.
		const std::string					_password; // server password.
		const std::string					_host; // server ip 127.0.0.1
		bool								_isRun; // program is running?
		int									_serverFd; // server's fd number.
		std::vector<pollfd>					_pollfds;
		std::map<int, Client*>				_clients; // client's with fd numbers.
		std::map<std::string, Channel *>	_channels; // channel's vector.
		std::string							_serverName;
/* -------------------------------------------------------------------------- */
/* _________________________ MAIN FUCTIONS __________________________________ */
	private:
		Server( void ); // Default Constructor.
	public:
		Server( int argc, char **argv );
		~Server( void ); // Destructor.
		void	start( void );
		void	acceptClients( void );
		void	commandHandler( Client* at );
		void	removeChannel( std::string channelName );
		void	removeClient( int clientFd );
		void	quitReason( Client* client, std::string message );
		void	serverInfo( void );
/* -------------------------------------------------------------------------- */
/* _________________________ COMMANDS _______________________________________ */
	private:
		std::map<std::string, CmdFunc> t_cmdFunc;
		void	cap( Client*, std::vector<std::string> );
		void	join( Client*, std::vector<std::string> );
		void	nick( Client*, std::vector<std::string> );
		void	pass( Client*, std::vector<std::string> );
		void	quit( Client*, std::vector<std::string> );
		void	user( Client*, std::vector<std::string> );
		void	privmsg( Client*, std::vector<std::string> );
		void	ping( Client*, std::vector<std::string> );
		void	part( Client*, std::vector<std::string> );
		void	list( Client*, std::vector<std::string> );
		void	whois( Client*, std::vector<std::string> );
		void	who( Client*, std::vector<std::string> );
		void	mode( Client*, std::vector<std::string> );
		void	modeUser( Client* it, std::vector<std::string> tokenArr );
		void	modeChannel( Client* it, std::vector<std::string> tokenArr );
		void	info( Client*, std::vector<std::string> );
		void	kick( Client*, std::vector<std::string> );
		void	topic( Client*, std::vector<std::string> );
/* -------------------------------------------------------------------------- */
/* _________________________ SOCKET FUCTIONS ________________________________ */
	private:
		void	openSocket( void );
		void	createSocketAddress( void );
		void	startListening( void );
/* -------------------------------------------------------------------------- */
/* _________________________ UTILS __________________________________________ */
	private:
		bool								check( int argc );
		unsigned short						port( std::string argv );
		std::string							password( std::string argv );
		void								initCommands( void );
		void								addToPollfds( int fd,  short events, short revents );
		std::map<std::string, std::string>	splitMessage( std::string delimeter, std::string message );
		std::vector<std::string>			cmdMessage( std::string message );
		int									getClientFdByNickname( std::string name ); //Nickname'ye Client'in fd'sini döndürür.
		Client*								getClientByNickname( std::string name ); // Nickname'ye Client'in pointerını döndürür.
		Client* 							getClientByFd( int fd ); //Fd'ye client'in pointerini döndürür.
		std::string							getRegisteredChannels( Client* Client ); // Client'in kayıtlı olduğu channellerin string listesi dönecektir. 
		std::string							combineMessage( size_t i, std::vector<std::string> vectorMessage ); //splitMessage fonk. tam tersi.
		bool								isChannelAdmin( Client* client, Channel* channel ); // Client'in belirtilen Channel'in admini mi, kontrol ediliyor.
		bool								isChannelUser( Client* client, Channel* channel ); // Client'in belirtilen Channel'in kullanıcısı mı kontrol ediliyor.
		void								leaveAllChannel( Client* client );
		std::string							welcomeServer( void );
/* -------------------------------------------------------------------------- */	
};

#endif
