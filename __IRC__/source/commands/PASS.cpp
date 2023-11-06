#include "../include/Server.hpp"
#include "../include/Utilities.hpp"

int     Server::Pass(std::string &s, Client& cli) {
    if (cli.passcheku)
        return 1;
    else if  (this->checkPassword(s, cli)) {
        cli.passcheku = 1;
        write(cli.cliFd, "Password is correct\n", 20);
        Utilities::fd_write_color(1, "Client: " + std::to_string(cli.cliFd -3) + " Pass: " + cli.pass + " has the password correctly\n", GREEN);

        
        return 1;
    }
    else {
        return 0;
    }
    return 0;
}

int    Server::checkPassword(std::string& s, Client& c) {
    if (s == password) {
        c.pass = s;
        c.passcheku = 1;
    }

    return (s == password);
}