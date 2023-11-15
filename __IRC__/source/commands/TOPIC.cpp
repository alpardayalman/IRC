#include "Server.hpp"

int     Server::Topic(std::string &s, Client &cli) {

    std::vector<std::string> params;
    params = Utilities::tokenCmd(s, 1);
    int chanelidx = isClientIn(cli, params[0]);
    if (!chanelidx)
        return 0;
    if (this->chanels[chanelidx-1].op->nick != cli.nick) {
        Utilities::writeRpl(cli.cliFd, ERR_CHANOPRIVSNEEDED(cli.nick, params[0]));
        return 0;
    }
    for (ChanelIterator it = this->chanels.begin(); it != this->chanels.end(); ++it) {
        if (it->name == params[0]) {
            if(params[1].empty())
                Utilities::writeRpl(cli.cliFd, RPL_NOTOPIC(cli.nick, params[0]));
            else
                Utilities::writeAllRpl(this->getFds(), RPL_TOPIC(cli.nick, cli.ipAddr, params[0], (params[1][0]==':'?params[1].substr(1, params[1].length()):params[1])));
            it->topic = (params[1][0]==':'?params[1].substr(1, params[1].length()):params[1]);
            break;
        }
    }
    return(1);
}
