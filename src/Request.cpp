#include "Request.hpp"

Request::Request()
{
}

Request::Request(std::string buffer)
{
    this->buff = buffer;
}

Request::~Request()
{
}

void    Request::parseRequest()
{
    std::stringstream str(buff);
    std::string       str1;
    std::string       key;
    std::string       value;

    while (str)
    {
        getline(str, str1, '\n');
        std::stringstream   line(str1);
        getline(line, key, ' ');
        getline(line, value);
        this->request[key] = value;
        std::cout << key << request[key] << std::endl;
    }
}
