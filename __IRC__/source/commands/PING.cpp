#include "Server.hpp"

int     Server::Ping(std::string &s, Client &cli) {
    std::string png = ":" + cli.getPrefix() + RPL_PONG(s);
    write(this->server_fd, png.c_str(), png.length());
    return 1;
}
