#include "Bot.hpp"

bool	Bot::check( int argc )
{
	std::cout << YELLOW "Checking parameters..." END << std::endl;
	if (argc != 4)
		throw( std::invalid_argument(RED "Error: Invalid Argument Count\n"
		"Usage: './ircbot <host> <port> <password>'" END) );
	return (true);
}

unsigned short	Bot::port( std::string port )
{
	// for: ' $> ./ircbot "" 1234 '
	//		' $> ./ircbot '' 1234 '
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

std::string		Bot::password( std::string password )
{
	// for: ' $> ./ircbot 1234 "" '
	//		' $> ./ircbot 1234 '' '
	if (password.empty())
		throw( std::invalid_argument(RED "Error: Invalid Password\n"
		"Password cannot be empty." END) );
	std::cout << GREEN "Parameters okay." END << std::endl;
	return (password);
}

std::string	Bot::getPrefixBot()
{
	return Bot::_botNickname
		+ (Bot::_botUsername.empty() ? "" : "!~" + Bot::_botUsername)
		+ (Bot::_host.empty() ? "" : "@" + Bot::_host);
}