#ifndef BOT_HPP
# define BOT_HPP

# include <iostream>
# include <string>
# include <exception>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <netdb.h>
# include <cstring>
# include <limits>
# include <pthread.h>
# include <unistd.h>
# include <map>
# include <algorithm>

# include "../colors.hpp"
# include "../Channel.hpp"
# include "../Server.hpp"

# define MAX_BUFFER 1024

class Channel;

class Bot
{
/* _________________________ VARIABLES ______________________________________ */
	private:
		bool							_isCheck;
		unsigned short					_port; // 8888
		const std::string				_password; // asdf
		bool							_isRun;
		std::map<std::string, Channel*>	_channels;
	public:
		static int						_botFd;
		static std::string				_host; // 127.0.0.1
		static std::string				_botNickname;
		static std::string				_botUsername;
		static std::string				_botRealname;
/* -------------------------------------------------------------------------- */
/* _________________________ MAIN FUCTIONS __________________________________ */
	private:
		Bot( void );
	public:
		Bot( int argc, char **argv );
		~Bot();
		static std::map<std::string, std::string>	_dataBadWords; // Static function'dan ulasabilmemiz icin bu variable'yi de static tanimlamamiz gerekiyor.
		void			start( void );
		static void*	listen( void * ); // POSIX thread's recv() function.
/* -------------------------------------------------------------------------- */
/* _________________________ SET/GET FUNCTIONS ______________________________ */
		static int	getFd( void ) { return (Bot::_botFd); }
		static std::string	getPrefixBot();
/* -------------------------------------------------------------------------- */
/* _________________________ COMMANDS _______________________________________ */
		void	authenticate( void );
		static void	sendMessageToServer( std::string message );
		void	checkChannels( void );
		static std::vector<std::string>\
			tokenMessage( std::string message );
		void	onMessageReceive( std::string buffer );
		static std::map<std::string, std::string>\
			botSplitMessage( std::string delimeter, std::string message );
		static void			initBadWords( void );
		static std::string	scanNickname( std::string message );
		static std::string	toLowerCase( const std::string& str );
/* -------------------------------------------------------------------------- */
/* _________________________ SIGNAL FUCTIONS ________________________________ */
		static void	sigHandler( int signalNum );
/* -------------------------------------------------------------------------- */
/* _________________________ SOCKET FUCTIONS ________________________________ */
	private:
		void			openSocket( void );
		void			connectSocketAddress( void );
/* -------------------------------------------------------------------------- */
/* _________________________ UTILS __________________________________________ */
	private:
		bool			check( int argc );
		unsigned short	port( std::string argv );
		std::string		password( std::string argv );
/* -------------------------------------------------------------------------- */	
};

#endif
