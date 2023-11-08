#include "Server.hpp"
#include "Utilities.hpp"

int     Server::Pass(std::string &s, Client& cli) {  // pass hatali rpli var onu bul ve entegre et
    if (cli.passcheku)
        return 1;
    else if  (this->checkPassword(s, cli)) {
        cli.passcheku = 1;
        write(cli.cliFd, "Password is correct\n", 20);
        std::cout << GREEN << "Client: " + std::to_string(cli.cliFd -3) + " Pass: " + cli.pass + " has the password correctly\n" << WHITE << std::endl;
        
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