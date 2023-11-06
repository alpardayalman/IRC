#include "Server.hpp"

// #define DEBUG_USER

int     Server::User(std::string &s, Client &cli) {
    std::stringstream ss(s);
    ss >> s;
    if (!s.empty()) {
        cli.user = s;
    }
#ifdef DEBUG_USER
        std::cout << "user:" << cli.user << "|" << std::endl;
#endif
    return 0;
}
