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

// curl -X POST -H "Transfer-Encoding: chunked" --data-binary @- http://localhost:8080 < data.txt
// Command to send a chunked post request to the server. The body will be in the data.txt

Request::Request():  _type(DEFAULT), _buff(""), _reqUrl(""), _isUrlCgi(false), _postFlag(false)
{
}

Request::Request(std::string buffer):  _type(DEFAULT), _buff(buffer), _reqUrl(""), _isUrlCgi(false), _postFlag(false)
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

void	Request::parsePostBody()
{
    std::string::size_type pos = 0;
	std::string	req = _buffCopy;
	if ((pos = _buffCopy.find("\r\n\r\n")) != std::string::npos && _request["content-length:"].empty() == false && _postFlag == false)
	{
        string cont_length = _request["content-length:"];
		std::string	body = req.substr(pos, ft::from_string<int>(cont_length));
		std::string::size_type pos1 = 0;
		while ((pos1 = body.find("\r\n", pos1)) != std::string::npos)
		{
			body.replace(pos1, 2, "\n");
			pos1 += 1; // Move past the replaced '\n' to avoid an infinite loop
		}
		_postBody = body;
		std::cout << "===========================================================================" << std::endl;
		std::cout << "{" << body << "}" << std::endl;
		std::cout << "===========================================================================" << std::endl;
		_postFlag = true;
	}
}

void	Request::parseHexReqUrl()
{
	unsigned int pos = 0;
	int hex_num = 0;
	std::string temp;
	std::string converted_hex;

	while(this->_reqUrl.find('%') != std::string::npos)
	{
		pos = this->_reqUrl.find_first_of('%');
		hex_num = hexadecimalToDecimal(this->_reqUrl.substr(pos + 1, 2));
		converted_hex.clear();
		converted_hex = char(hex_num);
		this->_reqUrl.replace(pos, 3, converted_hex);
	}
}

void	Request::parseQueryUrl()
{
	unsigned int pos = 0;
		bool	flag = false;
	pos = this->_reqUrl.find_first_of('?');
	if (!pos)
		return;
	std::string str;
	str = this->_reqUrl.substr(pos + 1, (this->_reqUrl.length() - pos));

    std::stringstream str1(str);
    std::string       pair;
	while (getline(str1, pair, '&'))
	{
		std::stringstream line(pair);
		std::string			key;
		std::string			value;

		getline(line, key, '=');
		getline(line, value);
		key = strToUpper(key);
		std::map<std::string, std::string>::iterator it;
		for (it = _queryMap.begin(); it != _queryMap.end(); ++it)
		{
			if (key == it->first)
				flag = true;
		}
		if (flag)
			continue;
		_queryMap[key] = value;
		// std::cout << key << "{" << _queryMap[key] << "}\n";
	}	
}

void    Request::parseRequest()
{

    std::string::size_type pos = 0;
	this->_buffCopy = _buff;
	std::cout << _buff << std::endl;
    while ((pos = _buff.find("\r\n", pos)) != std::string::npos)
    {
        _buff.replace(pos, 2, "\n");
        pos += 1;
    }
    std::stringstream str(_buff);
    std::string       str1;
    std::string       key;
    std::string       value;
    bool              first = true;


    while (getline(str, str1, '\n'))
    {
        if ((str1.empty() || isWhiteSpace(str1)) && _type == GET)
            continue;
		// if (_type == POST)
		// {
		// 	parsePostBody();
		// 	return ;
		// }
        std::stringstream   line(str1);
        getline(line, key, ' ');
        getline(line, value);
		this->_request[key] = value;
        if (first)
        {
            std::stringstream   url(value);
            getline(url, _reqUrl, ' ');
			parseHexReqUrl();
			parseQueryUrl();
			if (key.find("GET") != std::string::npos)
				this->_type = GET;
			else if (key.find("POST") != std::string::npos)
				this->_type = POST;
			else if (key.find("DELETE") != std::string::npos)
				this->_type = DELETE;
			std::size_t	pos_idx = _reqUrl.find("/cgi-bin");
			if (pos_idx != std::string::npos)
				this->_isUrlCgi = true;
            first = false;
        }
		if (key == "Host:")
			this->_host = value;
        // std::cout << key << "{" << _request[key] << "}\n";
    }
}

std::map<std::string, std::string>	Request::parseUnderScore()
{
	std::map<std::string, std::string> mapCopy = _request;
	std::map<std::string, std::string> mapC;
	std::map<std::string, std::string>::iterator it;
	for (it = mapCopy.begin(); it != mapCopy.end(); ++it)
	{
		std::string key = replaceChar(it->first);
		key = strToUpper(key);
		mapC[key] = it->second;
	}
	return (mapC);
}

std::map<std::string, std::string> Request::modifyEnv(std::map<std::string, std::string> env)
{
	std::map<std::string, std::string> updatedReq = parseUnderScore();
	std::map<std::string, std::string>::iterator it1;
	for (it1 = updatedReq.begin(); it1 != updatedReq.end(); ++it1)
	{

		env[it1->first] = it1->second;
	}
	for (it1 = _queryMap.begin(); it1 != _queryMap.end(); ++it1)
	{
		env[it1->first] = it1->second;
	}
	return env;
}

int Request::hexadecimalToDecimal(string hexVal)
{
    int len = hexVal.size();
    int base = 1;
    int dec_val = 0;

    for (int i = len - 1; i >= 0; i--) {
        if (hexVal[i] >= '0' && hexVal[i] <= '9') {
            dec_val += (int(hexVal[i]) - 48) * base;
            base = base * 16;
        }
        else if (hexVal[i] >= 'A' && hexVal[i] <= 'F') {
            dec_val += (int(hexVal[i]) - 55) * base;
            base = base * 16;
        }
    }
    return dec_val;
}

std::map<std::string, std::string> Request::getRequest() const
{
	return (_request);
}

std::string Request::getReqUrl() const
{
	return _reqUrl;
}

std::string Request::getPostBody() const
{
	return _postBody;
}

std::string	Request::getHost() const
{
	return _host;
}

e_request_type	Request::getReqType() const
{
	return (_type);
}

bool Request::isUrlCgi() const
{
	return _isUrlCgi;
}

std::string	Request::strToUpper(std::string str)
{
    for(size_t i = 0; i < str.length(); i++) {
        str[i] = toupper(str[i]);
    }
	return (str);
}

std::string Request::replaceChar(std::string str)
{
	for (size_t pos = str.find('-'); pos != std::string::npos; pos = str.find('-'))
	{
		str.replace(pos, 1, "_");
	}
	return(str);
}