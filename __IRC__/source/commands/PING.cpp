#include "Server.hpp"

Client getCli(std::vector<Client> arr, std::string &name) {
    Client  tmp;
    for (ClientIterator it = arr.begin(); it != arr.end(); it++) {
        if (name == it->nick) {
            tmp = (*it);
        }
    }
    return tmp;
}

int     Server::Ping(std::string &s, Client &cli) {
    std::vector<std::string> param;
    if (!s.empty()) {
        param = Utilities::splitString(s);
        if (s.at(0) == '0' && s.at(1) == '0') {
            param[0] = param[0].substr(2, param[0].length());
            Utilities::writeRpl(cli.cliFd, RPL_PING(cli.getPrefix(), param[0], s.substr(s.find("PING")+5, s.length()-1)));
        } else if (s.find(":PING") && param[1].size()) {//if PRIVMSG ile gelirse buraya girecek// PRIVMSG <target nick> :PING <komutun yollandigi saniye>//rpl_ping ile yollarsaniz gonderilen saniyeden o anki saniyeyi cikartip cliente yazdiriyor.
            Client target = getCli(this->clients, param[0]);

            Utilities::writeRpl(cli.cliFd, RPL_PING(cli.getPrefix(), param[0], s.substr(s.find("PING")+5, s.length()-1)));
            std::string msg = "00" + cli.nick + " :PING " + param[2];
            Server::Ping(msg, target);
            // Utilities::writeRpl(cli.cliFd, RPL_PING(cli.getPrefix(), param[0], s.substr(s.find("PING")+5, s.length()-1)));
        } else {//cmd handlerdan gelirse buraya girecek
            Utilities::writeRpl(clients[0].cliFd, RPL_PING(cli.getPrefix(), std::string("FT_IRC_SERVER"), param[0]));
        }
    }
    return 1;
}
