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

std::string Utilities::infoMessage(void) {

    std::string msg;

    msg += "* 42 Ecole IRC Project \n";
    msg += "* Based on the original code written by Jarkko Oikarinen \n";
    msg += "* Copyright 1988, 1989, 1990, 1991 University of Oulu, Computing Center \n";
    msg += "*  \n";
    msg += "* This program is free software; you can redistribute it and/or \n";
    msg += "* modify it under the terms of the MIT License as \n";
    msg += "* published by the Free Software Foundation; either version 1, or \n";
    msg += "* (at your option) any later version. \n";
    msg += "*  \n";
    msg += "* Developers: \n";
    msg += "* - Alp Arda Yalman \n";
    msg += "* - Emircan Kaymaz \n";
    msg += "* - Talha Açıkgöz \n";
    msg += "* - Yiğit Sarp Erkıralp \n";
    msg += "* - Yunus Emre Aktaş \n";
    msg += "*  \n";
    msg += "* Project Information: \n";
    msg += "* - Start Date: 1/11/2023 \n";
    msg += "* - Project Name: 42 Ecole IRC Project \n";
    msg += "* - Duration: Approximately two weeks \n";
    msg += "*  \n";
    msg += "* Thanks to the following individuals for their contributions: \n";
    msg += "* - zoovely             Seullu Abla \n";
    msg += "* - gsever              Gorkem Sever \n";
    msg += "* - akaraca             Ahmet Karaca \n";
    msg += "* - oyumusak            Omer Yumusak (Socket man) \n";
    msg += "*  \n";
    msg += "* Online Since: Tue Nov 14 09:42:00 2023 \n";
    msg += "* End of /INFO list. \n";

    return msg;
}