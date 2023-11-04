#include "Server.hpp"

#define debugUSER

int     Server::User(std::string &s, Client &cli) {
    std::stringstream ss(s);
    ss >> s;
    if (!s.empty()) {
        cli.user = s;
    }
#ifdef debugUSER
        std::cout << "user:" << cli.user << "|" << std::endl;
#endif
    return 0;
}
