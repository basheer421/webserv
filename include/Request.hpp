#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <string>
#include <map>
#include <sstream>

class Request
{
    private:
        std::map<std::string, std::string>  request;
        std::string                         buff;

    public:
        Request();
        Request(std::string buffer);
        ~Request();
        void    parseRequest();
};

#endif