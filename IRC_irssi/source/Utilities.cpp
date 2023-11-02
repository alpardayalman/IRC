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
    std::cout << msg << std::endl;
    write(fd, msg.c_str(), msg.length());
}