#pragma once
#include <iostream>
#include <vector>

class Client
{
    private:
        std::string identifier;
    public:
        Client();
        ~Client();
};

class Server
{
    private:
        char name[63];
        std::vector<User>   connected;

    public:
        Server();
        ~Server();
};

class User
{
    private:
        std::string                 _username;
        char                        _nickname[9]; // availabilty.
        std::vector<std::string>    _oldNames; //
        int                         _mode; //mode infos
        bool                        _isOperator;
    public:
        void            whoIs(User const & target) const;
        std::string     getFullname() const;
        void            quit();
        User();
        ~User();
};


class Operator : public User
{
	private:
	/* data */
	public:
	void	disconnectServer();
	void	reconnectServer();
	void	removeUser();
	void	closeConnexionUserServer();
	Operator(/* args */);
	~Operator();
};

class Services
{
	private:
		/* data */
		char		_nickname[9];
		std::string	_serverName;
	public:
		Services(/* args */);
		~Services();
};

class Channel
{
	private:
		/* data */
		// it SHALL NOT contain any spaces (' '), a control G (^G or ASCII 7), a comma (','). 
		char	_name[50]; // (beginning with a '&', '#', '+' or '!' character)
		// Channel names are case insensitive.
	public:
		Channel(/* args */);
		~Channel();
};

class Message
{
	private:
		/* data */
		std::string		_sender;
	public:
		std::string		getSender();
		Message(/* args */);
		~Message();
};


