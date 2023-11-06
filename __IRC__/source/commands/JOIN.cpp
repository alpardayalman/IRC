#include "Server.hpp"
#include "stdio.h"

// Bu class mi what the hell, ama kb mergleme demistin dayanamadim. Buradan ilerleriz eline saglik xoxo. (Biseye benzet lutfen)
// Debug islerini Utilitieslere eklersen super olur.
// PASSwordu yazdiktan sonra sadece JOIN yazinca server dusuyor...

int  findChanel(std::string& name, std::vector<Chanel> chanels) {
    for (ChanelIterator it = chanels.begin(); it != chanels.end(); it++) {
        if (name == (*it).name)
            return 1;
    }
    return 0;
}

void showClients(Chanel &cha) {
    for (ClientIterator it = cha.clients.begin(); it != cha.clients.end(); it++) {
        std::cout << (*it).nick << "|";
    }
    std::cout << std::endl;
}

int isClientIn(Chanel &cha, int cliFd) {
    for (ClientIterator it = cha.clients.begin(); it != cha.clients.end(); it++) {
        if ((*it).cliFd == cliFd)
            return 1;
    }
    return 0;
}

// #define ShowUser
#define DebugJ

int    Server::Join(std::string &s, Client& cli) {
    std::stringstream ss(s);
    std::string chaName, key;
    if (!s.empty()) {
        ss >> chaName;
        ss >> key;
        // chaName = chaName.substr(1, chaName.size());
            if (findChanel(chaName, this->chanels)) {
                for (ChanelIterator it = chanels.begin(); it != chanels.end(); it++) {
                    if (chaName == (*it).name) {//if chanel is exist try ot join chanel
                        if (!isClientIn((*it), cli.cliFd)) {
                            (*it).clients.push_back(cli);
                            std::string sendi = RPL_JOIN(cli.nick, cli.ipAddr, chaName);
                            write(cli.cliFd, sendi.c_str(), sendi.size());
                        }//client is in chanel or not
                        else
                            Utilities::fd_write_color(cli.cliFd, "You already in the chanel\n", CYAN);
                    }
                }
            } else {//if chanel is not exist create chanel and add client to chanel
                Chanel  newChanel(chaName);
                this->chanels.push_back(newChanel);
                this->chanels.back().clients.push_back(cli);
                std::string sendi = RPL_JOIN(cli.nick, cli.ipAddr, chaName);
                write(cli.cliFd, sendi.c_str(), sendi.size());
                // Bu tarzda bir sender fonksiyonu mantikli olur (line 59). atilan tum mesajlari terminale bastirabiliriz renk ile.
                // Ayni sekilde gonderilen mesajlarada bakmak saglikli olacaktir debug icin.
                // RPL_JOIN_NOV_6 -> test etmek icin yaptim, okulda devam ederiz normal #define'dan farki ip'si sabit. (Utilities.hpp)
                // : ! @ # neyi simgeliyor daha iyi anlamlastirmak GEREX.
            }
#ifdef ShowUser
    showClients(chanels.back());
#endif
    } else {//if join command input empty write this message
        Utilities::fd_write_color(cli.cliFd, "you cannot joined chanel cause use command correct JOIN <chanel name>\n", CYAN);
    }
    return 0;
}