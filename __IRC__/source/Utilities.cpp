#include "../include/Utilities.hpp"

int Utilities::checkPortNumber(char *port_number) {
    for (int i = 0; port_number[i]; i++) {
        if (!isdigit(port_number[i]))
            return 0;
    }
    return 1;
}

std::string Utilities::ltrim(const std::string &s) {
    size_t start = 0;
    while (start < s.length() && std::isspace(s[start])) {
        start++;
    }
    return s.substr(start);
}

std::string Utilities::rtrim(const std::string &s) {
    size_t end = s.length();
    while (end > 0 && std::isspace(s[end - 1])) {
        end--;
    }
    return s.substr(0, end);
}

std::string Utilities::trim(const std::string &s) {
    return rtrim(ltrim(s));
}

void Utilities::fd_write_color(int fd, const std::string &s, const char *color) {
    std::string msg = color + s;
    msg = msg + WHITE;
    // std::cout << msg << std::endl;
    write(fd, msg.c_str(), msg.length());
}

void Utilities::write_ascii_art(void){
/*         std::cout << "    /\\_/\\   " << std::endl;
        std::cout << "   ( o.o )  " << std::endl;
        std::cout << "    > ^ <   " << std::endl; */
        std::cout << " .----------------------------------------."<< std::endl;
        std::cout << ":      __                                  :" << std::endl;
        std::cout << ":   =='_))  __-:!:- (New client connected) :" << std::endl;
        std::cout << ":     ,.' .'  ))-:!:-                      :" << std::endl;
        std::cout << ":    ((_,'  .'-:!:-                        :" << std::endl;
        std::cout << ":   ~^~~~^~~^~~~^~                         :" << std::endl;
        //std::cout << " `----------------------------------------'  " << std::endl;
        for (int i = 0; i < 45; ++i) {
        // Önceki karakteri temizle
        std::cout << '\r';

        for (int j = 0; j < i; ++j) {
            std::cout << '-';
        }

        std::cout << std::flush;

        // Bekleme süresi ekleyerek animasyonu yavaşlatabilirsiniz
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    std::cout << std::endl;
}

std::vector<std::string> Utilities::tokenNewline(std::string& s) {
    std::stringstream           ss(s);
    std::vector<std::string>    arr;
    for (; std::getline(ss, s, '\n');) {
        s = Utilities::trim(s);
        arr.push_back(s);
    }
    return arr;
}

std::vector<std::string> Utilities::tokenCmd(std::string& s) {
    std::istringstream iss(s);
    std::vector<std::string> parameters;
    int paramlen;
    std::string param;
    iss >> param;
    parameters.push_back(param);
    paramlen = param.length();
    s.erase(0, paramlen + 1);
    parameters.push_back(s);
    return parameters;
}