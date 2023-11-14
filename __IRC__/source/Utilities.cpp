#include "Utilities.hpp"

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

void Utilities::write_ascii_art(void){

        std::cout << " .----------------------------------------."<< std::endl;
        std::cout << ":      __                                  :" << std::endl;
        std::cout << ":   =='_))  __-:!:- (New client connected) :" << std::endl;
        std::cout << ":     ,.' .'  ))-:!:-                      :" << std::endl;
        std::cout << ":    ((_,'  .'-:!:-                        :" << std::endl;
        std::cout << ":   ~^~~~^~~^~~~^~                         :" << std::endl;
        std::cout << " `----------------------------------------' " << std::endl;
}
/*
    Divide the string from the lines. And return tokenized version of every lines.
*/
std::vector<std::string> Utilities::tokenNewline(std::string& s) {
    std::stringstream           ss(s);
    std::vector<std::string>    arr;
    for (; std::getline(ss, s, '\n');) {
        s = Utilities::trim(s);
        arr.push_back(s);
    }
    return arr;
}
/*
    Divide the string from the first space. And return the first half as cmd[0] and second as cmd[1].
    Int Trim added if we want to trim both of them or not.
*/
std::vector<std::string> Utilities::tokenCmd(std::string& s, int Trim) {
    std::istringstream iss(s);
    std::string tmp = s;
    std::vector<std::string> parameters;
    int paramlen;
    std::string param;
    iss >> param;
    parameters.push_back(Trim?Utilities::trim(param):param);
    paramlen = param.length();
    tmp.erase(0, paramlen + 1);
    parameters.push_back(Trim?Utilities::trim(tmp):tmp);
    return parameters;
}


std::vector<std::string> Utilities::splitString(const std::string& input) {
    std::istringstream iss(input);
    std::vector<std::string> result;

    do {
        std::string word;
        iss >> word;
        result.push_back(word);
    } while (iss);

    result.pop_back();

    return result;
}

bool    Utilities::checkChannel(std::string& s) {
    if(s.size() > 50)
        return false;
    for(int i = 0; s[i]; i++) {
        if(s[i] == ' ' || s[i] == 7 || s[i] == ',')
            return false;
    }
    return true;
}

void    Utilities::writeRpl(int fd, std::string msg) {
    (write(fd, msg.c_str(), msg.length()) < 0) ? 
        (std::cout << "dirtttitirti" << std::endl) : std::cout;
}

void    Utilities::writeAllRpl(std::vector<int> fd, std::string msg) {
    for(std::vector<int>::iterator it = fd.begin(); it != fd.end(); ++it) {
        Utilities::writeRpl((*it), msg);
    }
}


std::string     Utilities::intToString(int myInteger) {

    std::stringstream ss;
    ss << myInteger;
    std::string myString = ss.str();

    return myString;
}