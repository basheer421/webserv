/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 18:01:59 by mkhan             #+#    #+#             */
/*   Updated: 2023/07/29 18:01:59 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(): _buff(""), _reqUrl(""), _isUrlCgi(false)
{
}

Request::Request(std::string buffer): _buff(buffer), _reqUrl(""), _isUrlCgi(false)
{
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

	// Replace all occurrences of '\r\n' with '\n' in the _buff string
    std::string::size_type pos = 0;
    while ((pos = _buff.find("\r\n", pos)) != std::string::npos)
    {
        _buff.replace(pos, 2, "\n");
        pos += 1; // Move past the replaced '\n' to avoid an infinite loop
    }

    std::stringstream str(_buff);
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
        this->_request[key] = value;
        if (first)
        {
            std::stringstream   url(value);
            getline(url, _reqUrl, ' ');
			std::size_t	pos_idx = _reqUrl.find("/cgi-bin");
			if (pos_idx != std::string::npos)
				this->_isUrlCgi = true;
            std::cout << "Requested Url ===========> " << _reqUrl << " === _isUrlCgi ====> " << _isUrlCgi << std::endl;
            first = false;
        }
        std::cout << key << "{" << _request[key] << "}\n";
    }
}

std::map<std::string, std::string> Request::getRequest() const
{
	return (_request);
}

std::string Request::getReqUrl() const
{
	return _reqUrl;
}
bool Request::isUrlCgi() const
{
	return _isUrlCgi;
}
