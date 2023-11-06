#include "Server.hpp"

int     Server::Cap(std::string &s, Client &cli) {
    if (s == "CAP LS")
        cli.isCapt = HEX;
    else if (s == "CAP NC")
        cli.isCapt = NC;
    else
        return 0;
    return 1;
}
