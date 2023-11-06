#pragma once
#include <iostream>
#include <unistd.h>
#include <string>
#include <thread>
#include <chrono>
#include <sstream>
#include <vector>

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
#define RPL_JOIN_NOV_6(nick, channel)				":" + nick + "!" + nick + "@" + "127.0.0.1" + " JOIN " + channel + "\r\n" // test icin IP vermedim

// ============= Define ============

// ==========debug===========
#define  tokenNewlinedeb
class Utilities {
    private:
        Utilities() {}
        ~Utilities() {}
        static std::string                      ltrim(const std::string &s);
        static std::string                      rtrim(const std::string &s);

    public:
        static int                              checkPortNumber(char *port_number);
        static std::string                      trim(const std::string &s);
        static std::vector<std::string>         tokenCmd(std::string& s);
        static std::vector<std::string>         tokenNewline(std::string& s);

        // ART & DESIGN ®
        static void                             write_ascii_art(void);
        static void                             fd_write_color(int fd, const std::string &s, const char *color);
};
