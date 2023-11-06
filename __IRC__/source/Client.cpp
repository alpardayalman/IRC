#include "Client.hpp"

void    Client::colorWrite(std::string &msg, const char *color) {
    msg = color + msg + WHITE;
    write(this->cliFd, msg.c_str(), msg.length());
}