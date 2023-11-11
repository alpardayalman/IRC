#include "Server.hpp"

int Server::Mode(std::string &s, Client &cli) {
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
    if (it->op->user != cli.user) { // No modarator.
        return 0;
    }

    if (cmd[1] == "+k") {
        if (cmd.size() == 3) {
            it->key = cmd[2];
            it->keycode |= K_CODE;
        }
    }
    if (cmd[1] == "+l") {
        if (cmd.size() == 3) {
            it->users = atoi(cmd[2].c_str());
            it->keycode |= L_CODE;
        }
    }
    if (cmd[1] == "+o") {
        if (cmd.size() == 3) {
            Server::Op(s, cli);
        }
    }
    if (cmd[1] == "-k") {
        if (cmd.size() == 3 && it->key == cmd[3]) {
            it->key = "";
            it->keycode ^= K_CODE;
        }
    }
    if (cmd[1] == "-l") {
        if (cmd.size() == 2) {
            it->keycode ^= L_CODE;
        }
    }
    return 0;
}
