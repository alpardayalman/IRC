#pragma once
#include <iostream>

class Utilities {
    private:
        Utilities() {}
        ~Utilities() {}
        static std::string ltrim(const std::string &s);
        static std::string rtrim(const std::string &s);

    public:
        static int checkPortNumber(char *port_number);
        static std::string trim(const std::string &s);
};