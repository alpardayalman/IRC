#include "Client.hpp"

void    Client::colorWrite(std::string &msg, const char *color) {
    msg = color + msg + WHITE;
    write(this->cliFd, msg.c_str(), msg.length());
}


std::string	Client::getPrefix() const // calismazsa source bu.
{
	return this->nick
		+ (this->user.empty() ? "" : "!~" + this->user)
		+ (this->ip.empty() ? "" : "@" + this->ip);
}