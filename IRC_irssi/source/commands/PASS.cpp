#include "../include/Server.hpp"

int     Server::Pass(std::string &s, Client& cli) {
    if (cli.passcheku)
        return 1;
    else if  (this->checkPassword(s, cli)) {
        cli.passcheku = 1;
        write(cli.cliFd, "Password is corect\n", 20);
        std::cout << "Client: " << cli.cliFd <<  " has the password correctly"<<std::endl;
    }
    else {
        return 0;
    }
    return 1;
}
