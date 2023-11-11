#include "Server.hpp"

class Exception : public std::exception {
    private:
        const char* _msg;
    public:
        Exception(const char* msg) : _msg(msg) {}
        virtual const char* what() const throw() {
            static std::string msg = std::string(RED) + "Error: " + this->_msg + RESET;
            return msg.c_str(); 
        }
};