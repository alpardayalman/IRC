#include "Server.hpp"

int     Server::Ping(std::string &s, Client &cli) {
    std::vector<std::string> param;
    if (!s.empty())
        param = Utilities::tokenCmd(s, 1);
    if (s.find(":PING") && param[1].size()) {//if PRIVMSG ile gelirse buraya girecek// PRIVMSG <target nick> :PING <komutun yollandigi saniye>//rpl_ping ile yollarsaniz gonderilen saniyeden o anki saniyeyi cikartip cliente yazdiriyor.
        std::vector<std::string> arr = Utilities::tokenCmd(s, 1);
        arr = Utilities::tokenCmd(arr[1], 0);
        arr = Utilities::tokenCmd(arr[1], 0);
        cli.messageBox.push_back(RPL_PING(cli.getPrefix(), arr[0], s.substr(s.find("PING")+5, s.length()-1)));
    } else {//cmd handlerdan gelirse buraya girecek
        cli.messageBox.push_back(RPL_PING(cli.getPrefix(), cli.nick, param[0]));
    }
    FD_SET(cli.cliFd, &this->writeFds);
    return 1;
}
