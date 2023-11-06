#include "../include/Server.hpp"
#include "../include/Utilities.hpp"

int     Server::Pass(std::string &s, Client& cli) {
    // std::cout << "Pass command is called." << s << std::endl;
    if (cli.passcheku)
        return 1;
    else if  (this->checkPassword(s, cli)) {
        cli.passcheku = 1;
        write(cli.cliFd, "Password is correct\n", 20);
        Utilities::fd_write_color(1, "Client: " + std::to_string(cli.cliFd) + " Pass: " + cli.pass + " has the password correctly\n", GREEN);

        
        return 1;
    }
    else {
        return 0;
    }
    return 0;
}

int    Server::checkPassword(std::string& s, Client& c) {
    // CAP LS \n PASS <PSW> \n NICK <NICK> \n USER <USR> <USR> <IP> :<REALNAME>
    int res = 0;
    std::vector<std::string> arr;
    std::stringstream iss(s);
    if (!s.find("CAP LS")){
        for (; std::getline(iss, s, '\n'); ) {
            Utilities::trim(s);
            std::stringstream pss(s);
            for (; pss >> s;) {
                Utilities::trim(s);
                if (!s.compare("PASS")) {
                    pss >> s;
                    c.pass = s;
                    Utilities::trim(c.pass);
                    if (c.pass == password) {
                        res=1;
                    }
                }
                else if (!s.compare("NICK")) {
                    pss >> s;
                    Server::Nick(s, c);
                }
                else if (!s.compare("USER")) {
                    pss >> s;
                    Server::User(s, c);
                }
            }
        }
    }
    else {
        std::stringstream ss(s);
#ifdef DEBUG_PASS
        std::cout << "s:" << s << '\n';
#endif
        std::string cmd, pass;
        ss >> cmd;
        ss >> pass;
        if (!cmd.compare("PASS")) {
            c.pass = pass;
            if (c.pass == password)
                res=1;
        }
#ifdef DEBUG_PASS
        for (int i = 0; c.pass[i]; i++) {
            printf("%d ", c.pass[i]);
        }
        fflush(stdout);
#endif
    }

    return res;
}