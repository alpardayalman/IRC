#include "Server.hpp"

#define DEBUG_USER

int     Server::User(std::string &s, Client &cli) {
    std::stringstream ss(s);
    ss >> s;
    if (!s.empty()) {
        cli.user = s;
    }
    ss >> s;
    ss >> s;
    if (!s.empty()) {
        cli.ip = s;
    }
    return 0;
}
