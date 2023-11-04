#ifndef	CLIENT_HPP
# define CLIENT_HPP

# include <vector>
# include <string>
# include <iostream>
# include <sys/socket.h>
# include <errno.h>
# include <sstream>

# include "colors.hpp"
# include "Channel.hpp"

class Channel;

/*
	Server'da gelişi güzel kontrol eklemek yerine client'ın hangi aşamalarda olduğunu tutarlı şekilde takip etmek için eklendi.
*/
enum IRCstatus{
	CONNECTING, // Bu durum, IRC client'ın IRC server'a bağlanmaya çalıştığı durumu temsil eder.
	HANDSHAKE, // Bu durum, bağlantının kurulduğu ve IRC client'in IRC server ile bir kimlik oluşturmaya çalıştığı durumu temsil eder.
	RENICK, // Bu durum, ilk bağlantıdaki client'ın nick'i aynı ise tekrardan nick isteminde bulunmak için tanımlanmaktadır.
	AUTHENTICATED, // Bu durum, IRC client'in IRC server tarafından kimlik doğrulaması yapıldığı durumu temsil eder.
	DISCONNECTED, // Bu durum, bağlantının sonlandırıldığı durumu temsil eder.
};

class Client
{
	private:
		int						_fd;
		unsigned short			_port;
		std::string				_hostname;
		std::string				_nickname;
		std::string				_username;
		std::string				_realname;
		bool					_havePassword; // server'a bağlanırken password olmadığından dolayı pass kontrolü yapamıyoruz, server herzaman passwordlü olduğundan dolayı bu şekilde kullanıcı password girmiş mi girmemiş mi kontrol edebiliyoruz.
		int						_ircstatus; // enum değerlerini tutmak için bulunuyor.
		std::vector<Channel*>	_registeredChannels; // Kullanıcının kayıtlı olduğu channel'lara erişmek daha kolay olsun diye eklendi.
		std::string				_clientModes; // Client'in modlarını tutar.

	public:
		Client( int fd, unsigned short port, const std::string hostname );
		~Client();

		int				getIRCstatus( void ) { return (this->_ircstatus); }
		int				getFd( void ) { return (this->_fd); }
		unsigned short	getPort( void ) { return (this->_port); }
		std::string		getHostname( void ) { return (this->_hostname); }
		std::string		getNickname( void ) { return (this->_nickname); }
		std::string		getUsername( void ) { return (this->_username); }
		std::string		getRealname( void ) { return (this->_realname); }
		bool			getPasswordStatus( void ) { return (this->_havePassword); }
		std::string		getPrefix() const;
		std::string		getClientMods( void ) { return (this->_clientModes); }

		void	setIRCstatus( int status ) { this->_ircstatus = status; }
		void	setHostname( std::string name ) { this->_hostname = name; }
		void	setNickname( std::string name ) { this->_nickname = name; }
		void	setUsername( std::string name ) { this->_username = name; }
		void	setRealname( std::string name ) { this->_realname = name; }
		void	setPasswordStatus( void ) { this->_havePassword = true; }
		void	setClientMods( std::string mod );
		void	sendMessageFd( std::string message );
		void	sendWelcomeMessage( std::string message );

		void	registerChannel( Channel* channel ); // Client içinde bulunan _registeredChannels'e Channele eklemek için vardır.
		void	unregisterChannel( Channel* channel ); // Client içinde bulunan _registeredChannels'den Channele kaldırmak için vardır.
		std::vector<Channel*> getRegisteredChannels( void ) { return (this->_registeredChannels); } // Client'tin kayıtlı olduğu clientlerin listesini döndürür.
		bool	isRegisteredChannel( std::string channelName ); // Client'in o kanala kayıtlı olup olmadığı kontrol eder.
		void	clearRegisteredChannels( void ) { this->_registeredChannels.clear(); } // Client quit attığı zaman kayıtlı olduğu tüm channelerden çıkarmak için yapıyoruz.
};

#endif
