#include "Server.hpp"

int Server::Mode(std::string &s, Client &cli) {
    (void)s;
    (void)cli;
    std::vector<std::string> cmd = Utilities::splitString(s);
    if (cmd.size() > 3)
        return 0;
    if (cmd.size() == 1)
        return 0;
    // operator olma durumunu kontrol etmek gerekiyor.
    // K durumu
    ChanelIterator it = this->chanels.begin();
    for (; it != this->chanels.end(); ++it)
        if (it->name == cmd[0])
            break;
    if (it == this->chanels.end()) // No chanel name
        return 0;
    if (it->op->user != cli.user) // No modarator.
        return 0;

    if (cmd[1] == "+k") {
        if (cmd.size() == 3) {
            it->key = cmd[2];
            it->keycode |= K_CODE;
            Utilities::writeRpl(cli.cliFd, RPL_MODE(cli.nick, cmd[0], "+k", cmd[2]));
            std::cout << "key is added" << std::endl;
        }
    }
    if (cmd[1] == "+l") {
        if (cmd.size() == 3) {
            it->users = atoi(cmd[2].c_str());
            it->keycode |= L_CODE;
            Utilities::writeRpl(cli.cliFd, RPL_MODE(cli.nick, cmd[0], "+l", cmd[2]));
        }
    }
    if (cmd[1] == "-k") {
        if (cmd.size() == 3 && it->key == cmd[2]) {
            it->key = "";
            it->keycode ^= K_CODE;
            Utilities::writeRpl(cli.cliFd, RPL_MODE(cli.nick, cmd[0], "-k", ""));
            std::cout << "key is deleted" << std::endl;
        }
    }
    if (cmd[1] == "-l") {
        if (cmd.size() == 2) {
            it->keycode ^= L_CODE;
            //Utilities::writeRpl(cli.cliFd, RPL_MODE(cli.nick, cmd[0], "-l", ""));
        }
    }
    return 0;
}
