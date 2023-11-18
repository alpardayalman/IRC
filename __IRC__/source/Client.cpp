#include "Client.hpp"

std::string Client::getPrefix() const {
	return this->nick + (this->user.empty() ? "" : "!~" + this->user) + (this->ip.empty() ? "" : "@" + this->ip);
}

const Client &Client::operator=(const Client &copy) {
	this->messageBox = copy.messageBox;
	this->cliFd = copy.cliFd;
	this->port = copy.port;
	this->ip = copy.ip;
	this->user = copy.user;
	this->nick = copy.nick;
	this->pass = copy.pass;
	this->isCapt = copy.isCapt;
	this->passcheku = copy.passcheku;
	this->buffer = copy.buffer;
	return (*this);
}