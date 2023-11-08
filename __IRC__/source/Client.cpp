#include "Client.hpp"

std::string Client::getPrefix() const {
	return this->nick + (this->user.empty() ? "" : "!~" + this->user) + (this->ip.empty() ? "" : "@" + this->ip);
}