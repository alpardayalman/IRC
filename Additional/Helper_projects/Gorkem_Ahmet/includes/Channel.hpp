#ifndef	CHANNEL_HPP
# define CHANNEL_HPP

# include <map>
# include "Client.hpp"
# include "utils.hpp"
# include <algorithm>

class Client;

typedef	std::vector<Client *>::iterator itChannelAdmins;
typedef	std::vector<Client *>::iterator itChannelClients;

class Channel
{
	private:
		std::string				_name;
		int						_clientCount;
		std::vector<Client*>	_admins; // Birden fazla admin.
		std::string				_channelTopic; // Channel'e bağlantı kurulduğunda konu başlığının belirlenmesinde yardımcı oluyor.
		std::string				_channelModes; // Channel'in modlarını tutar.

		std::string				_k; // Channel şifresini belirliyor
		int						_l; // Channel'a bağlanacak max client sayısını temsil ediyor.

	public:
		Channel( std::string name, std::string password);
		~Channel( void );
		std::vector<Client *>	_channelClients;

		std::string				getName( void ) { return (this->_name); }
		Client*					getAdmin( std::string nickname );
		std::vector<Client*>	getAdmins( void ) { return (this->_admins); }
		std::string 			getChannelTopic( void ) { return (this->_channelTopic); }
		int						getClientCount( void ) { return (this->_clientCount); }
		std::string				getPassword( void ) { return (this->_k); }
		int						getMaxClient( void ) { return (this->_l); }
		std::string 			getChannelMods( void ) { return (this->_channelModes); }

		void		setName( std::string &name ) { this->_name = name; }
		void		setChannelTopic( std::string topic ) { this->_channelTopic = topic; }
		void		setAdmins( std::vector<Client*> admins ) { this->_admins = admins; }
		void		setPassword( std::string password ) {this->_k = password; }
		void		setMaxClient( int maxClient ) {this->_l = maxClient; }
		void		setChannelMods( std::string mod );

		void		addClient( Client* client );
		void		addAdmin( Client* admin );

		void		removeClient( Client* client );
		void		removeAdmin( Client* admin );

		void		sendMessageBroadcast( std::string message );
		void		sendMessageBroadcast( Client* exclude, std::string message );
		void		channelUsers(Client* client, Channel* channel, std::string channelName ); // Channel'e girişte kullanıcıların listesini channel'e bastırıyor.

		Client*		searchAdmin( Client* client );
		Client*		searchClient( Client* client );
};

#endif
