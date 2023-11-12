#include "Server.hpp"

int     Server::Cap(std::string &s, Client &cli) {
    // std::cout << "Cap" << s << std::endl;
    if (s == "LS")
        cli.isCapt = HEX;
    else if (s == "NC")
        cli.isCapt = NC;
    else if (s == "BOT")
        cli.isCapt = BOT;
    else
        return false;
    return true;
}
