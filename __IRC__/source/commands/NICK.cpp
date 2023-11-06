#include "Server.hpp"

// #define DEBUG_NICK

int     Server::Nick(std::string &s, Client &cli) {
    std::stringstream ss(s);
    ss >> s;
    if (!s.empty()) {
        cli.nick = s;
    }
    return 0;
}
