#include "Request.hpp"

Request::Request()
{
}

Request::Request(std::string buffer)
{
    this->buff = buffer;
}

Request::Request(const Request &object)
{
    *this = object;
}

Request &Request::operator=(const Request &rhs)
{
    if (this != &rhs)
    {
        //
    }
    return (*this);
}

Request::~Request()
{
}

bool    Request::isWhiteSpace(std::string str1)
{
    for (size_t i = 0; i < str1.length(); i++)
        if (!isspace(str1[i]))
            return (false);
    return (true);
}

void    Request::parseRequest()
{
    std::stringstream str(buff);
    std::string       str1;
    std::string       key;
    std::string       value;
    bool              first = true;

    while (getline(str, str1, '\n'))
    {
        if (str1.empty() || isWhiteSpace(str1))
            continue;
        std::stringstream   line(str1);
        getline(line, key, ' ');
        getline(line, value);
        this->request[key] = value;
        if (first)
        {
            std::stringstream   url(value);
            getline(url, reqUrl, ' ');
            std::cout << "Requested Url ===========> " << reqUrl << std::endl;
            first = false;
        }
        std::cout << key << request[key] << std::endl;
    }
}

