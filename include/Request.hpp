#pragma once

#include <iostream>
#include <string>
#include <map>
#include <sstream>

class Request
{
    private:
        std::map<std::string, std::string>  request;
        std::string                         buff;
        std::string                         reqUrl;

    public:
        Request();
        Request(std::string buffer);
        Request(const Request &object);
        Request &operator=(const Request &rhs);
        ~Request();
        void    parseRequest();
        bool    isWhiteSpace(std::string    str1);
};
