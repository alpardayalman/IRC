#pragma once
#include <iostream>
#include <unistd.h>
#include <string>
#include <sstream>
#include <vector>
#include "Client.hpp"


// ============= color =============
# define BLACK		"\033[0;30m"
# define RED		"\033[0;31m"
# define GREEN		"\033[0;32m"
# define YELLOW		"\033[0;33m"
# define BLUE		"\033[0;34m"
# define PURPLE		"\033[0;35m"
# define CYAN		"\033[0;36m"
# define WHITE		"\033[0;37m"
# define END		"\033[m"
# define RESET		"\033[0m"
// ============= color =============

// ============= Define ============
#define RPL_JOIN(nick, ip, channel)					":" + nick + "!" + nick + "@" + ip + " JOIN " + channel + "\r\n"
#define RPL_NAMREPLY(nick, channel, users)			": 353 " + nick + " = " + channel + " :" + users + "\r\n"
#define RPL_ENDOFNAMES(nick, channel)               ": 366 " + nick + " " + channel + " :End of /NAMES list\r\n"
#define RPL_JOIN_NOV_6(nick, channel)				":" + nick + "!" + nick + "@" + "127.0.0.1" + " JOIN " + channel + "\r\n"
#define RPL_TOPIC(nick, ip, channel, topic)         ":" + nick + "!" + nick + "@" + ip + " TOPIC " + channel + " :" + topic + "\r\n"       
#define RPL_INVITE(nick, ip, channel, target)       ":" + nick + "!" + nick + "@" + ip + " INVITE " + target + " " + channel + "\r\n"  
#define RPL_PRIVMSG(source, target, message)		":" + source + " PRIVMSG " + target + " :" + message + "\r\n"
#define RPL_NOTICE(source, target, message)			":" + source + " NOTICE " + target + " :" + message + "\r\n"
#define RPL_NICK(nick, user, ip, newnick)			":" + nick + "!" + user + "@" + ip + " NICK :" + newnick + "\r\n"
#define RPL_KICK(source, channel, target, reason)	":" + source + " KICK " + channel + " " + target + " :" + reason + "\r\n"
#define RPL_PART(source, channel)					":" + source + " PART :" + channel + "\r\n"
#define RPL_PONG(source, token)						":" + source + " PING :" + token + "\r\n"
#define RPL_QUIT(source, message)                   ":" + source + " QUIT :" + message + "\r\n"
#define RPL_MODE(source, channel, modes, args)		":" + source + " MODE " + channel + " " + modes + " " + args + "\r\n"
#define RPL_INFO(source, info)                      ":" + source + " 371 * :" + info + "\r\n"

/* [WHOIS] */
#define RPL_WHOISUSER(nick, user, ip)				": 311 " + nick + " " + nick + " " + nick + " " + ip + " * :" + user + "\r\n"
#define RPL_WHOISSERVER(source, server)				": 312 " + source + " * :" + server + "\r\n"
#define RPL_ENDOFWHOIS(source, nick)				": 318 " + source + " " + nick + " :End of /WHOIS list" + "\r\n"
#define RPL_PING(source, nick, second)				":" + source + " PONG " + nick + " :"+ second + "\r\n"


# define RPL_LISTSTART(nick, numusers)			    ": 321 " + nick + " Channel : "+ numusers + "\r\n"
# define RPL_LIST(nick, channel, numusers, topic)   ": 322 " + nick + " " + channel + " " + numusers + " " + topic + "\r\n"
# define RPL_LISTEND(nick)						    ": 323 " + nick + " :End of /LIST\r\n"


#define RPL_WELCOME(source, msg)				    source + msg + "\r\n"

/* [ERROR MESSAGES] */
#define ERR_CHANNELISFULL(source, channel)			": 471 " + source + " " + channel + " :Cannot join channel (+l)" + "\r\n"           //JOIN
#define ERR_BADCHANNELKEY(source, channel)			": 475 " + source + " " + channel + " :Cannot join channel (+k)" + "\r\n"           //JOIN
#define ERR_NEEDMOREPARAMS(source, command)			": 461 " + source + " " + command + " :Not enough parameters" + "\r\n"              //MODE
#define ERR_CHANOPRIVSNEEDED(source, channel)		": 482 " + source + " " + channel + " :You're not the channel operator" + "\r\n"    //MODE
#define ERR_CHANOPKICK(source, channel)		        ": 482 " + source + " " + channel + " :You cannot KICK yourself" + "\r\n"           //MODE
#define ERR_NOSUCHCHANNEL(source, channel)			": 403 " + source + " " + channel + " :No such channel" + "\r\n"                    //MODE
#define ERR_NICKNAMEINUSE(source)				    ": 433 " + source + " " + source  + " :Nickname is already in use" + "\r\n"         //NICK
#define ERR_NICKNAMEEMPTY(source)				    ": 433 " + source + " " + source  + " :Nickname cannot empty" + "\r\n"              //NICK
#define RPL_NOTOPIC(nick, channel)                  ": 331 " + nick + " " + channel + " :No topic is set" + "\r\n"                      //TOPIC


class Utilities {
    private:
        Utilities() {}
        ~Utilities() {}
        static std::string                      ltrim(const std::string &s);
        static std::string                      rtrim(const std::string &s);

    public:
        static int                              checkPortNumber(char *port_number);
        static std::string                      trim(const std::string &s);
        static std::vector<std::string>         tokenCmd(std::string& s, int Trim);
        static std::vector<std::string>         tokenNewline(std::string& s);
        static std::vector<std::string>         splitString(const std::string& input);

        // ART & DESIGN ®
        static std::string                      write_ascii_art(void);
        static std::string                      infoMessage(void);

        //Channel check
        static bool                             checkChannel(std::string& s);
        static void                             writeRpl(int fd, std::string msg);
        static void                             writeAllRpl(std::vector<int> fd, std::string msg);


        static std::string                      intToString(int);
};
