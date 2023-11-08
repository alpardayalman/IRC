#include "Server.hpp"

            if (params[1].find("PING")) {
                std::string s = params[1].substr(5, params[1].length());
                Server::Ping(s, (*it));
                break;
            }

int     Server::Ping(std::string &s, Client &cli) {
    std::vector<std::string> param = Utilities::tokenCmd(s, 1);
    std::cout << "param0" << param[0] << " s:" << s << std::endl;
    std::string png = RPL_PING(param[0], cli.nick+" :" + param[0]);
    write(cli.cliFd, png.c_str(), png.length());
    return 1;
}
