#include "../include/Server.hpp"
#include "../include/Utilities.hpp"

int     Server::Pass(std::string &s, Client& cli) {
    if (cli.passcheku)
        return 1;
    else if  (this->checkPassword(s, cli)) {
        cli.passcheku = 1;
        write(cli.cliFd, "Password is correct\n", 20);
        std::cout << "Client: " << cli.cliFd <<  " has the password correctly"<<std::endl;
        return 1;
    }
    else {
        return 0;
    }
    return 1;
}

bool    Server::checkPassword(std::string& s, Client& c) { // BUNU GUZELLESTIRELIM ALLAH ASKINA
    // CAP LS
    // PASS <PSW>
    // NICK <NICK>
    // USER <USR> <USR> <IP> :<REALNAME>

    bool res = false;
    std::vector<std::string> arr;
    std::stringstream iss(s);
    if (!s.find("CAP LS")){
        for (; std::getline(iss, s, '\n'); ) {
            Utilities::trim(s);
            std::stringstream pss(s);
            for (; pss >> s;) {
                if (!s.compare("PASS")) {
                    pss >> s;
                    c.pass = s;
                    if (c.pass == password)
                        res=true;
                }
                else if (!s.compare("NICK")) {
                    pss >> s;
                    c.nick = s;
                }
                else if (!s.compare("USER")) {
                    pss >> s;
                    c.user = s;
                }
            }
        }
    }
    else {
        std::stringstream ss(s);
#ifdef DEBUG
        std::cout << "s:" << s << '\n';
#endif
        std::string cmd, pass;
        ss >> cmd;
        ss >> pass;
        if (!cmd.compare("/PASS")) {
            c.pass = pass;
            if (c.pass == password)
                res=true;
        }
#ifdef DEBUG
        for (int i = 0; c.pass[i]; i++) {
            printf("%d ", c.pass[i]);
        }
        fflush(stdout);
#endif
    }
    return res;
}