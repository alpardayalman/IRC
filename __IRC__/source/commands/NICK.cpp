#include "Server.hpp"

#define debugNICK

int     Server::Nick(std::string &s, Client &cli) {
    std::stringstream ss(s);
    ss >> s;
    if (!s.empty()) {
        cli.nick = s;
    }
#ifdef debugNICK
        std::cout << "nick:" << cli.nick << "|" << std::endl;
#endif
    return 0;
}
