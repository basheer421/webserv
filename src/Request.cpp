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

Request::Request():  _type(DEFAULT), _buff(""), _reqUrl(""), _isUrlCgi(false), _postFlag(false), _isFileUpload(false), _putCode("200"), _queryString(""), queryURl(""), _flagEnc(false)
{
	this->_contLen = 0;
}

Request::Request(std::string buffer):  _type(DEFAULT), _buff(buffer), _reqUrl(""), _isUrlCgi(false), _postFlag(false), _isFileUpload(false), _putCode("200"), _queryString(""), queryURl(""), _flagEnc(false)
{
	this->_contLen = 0;
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

void	Request::parseChunkedBody()
{
		std::string	req = _buffCopy;
		std::string::size_type pos2 = 0;
		std::string	sub_string;
		if ((pos2 = req.find("\r\n\r\n")) != std::string::npos)
			sub_string = req.substr(pos2 + 4, req.length() - pos2 - 8);

    	std::string::size_type pos1 = 0;
		while ((pos1 = sub_string.find("\r\n", pos1)) != std::string::npos)
		{
			sub_string.replace(pos1, 2, "\n");
			pos1 += 1;
		}
		std::stringstream res(sub_string);
		std::string line;
		std::string body;

		while (getline(res, line, '\n'))
		{
			if (line[0] == '0' && line.length() == 1)
				break ;
			std::string::size_type posN = line.find('\n');
			std::string	num = line.substr(0, posN);
			size_t body_len;
			if (num.find_first_not_of("0123456789") != std::string::npos)
			{
				std::stringstream ss;
				ss << std::hex << num;
				ss >> body_len;
			}
			else
				body_len = ft::from_string<int>(num);
			std::string appendBody;
			while (appendBody.length() < body_len)
			{
				getline(res, line, '\n');
				appendBody += line;
			}
			body += appendBody;
		}
		_postBody = body;
		// std::cout << "===========================================================================" << std::endl;
		// std::cout << "{" << body << "}" << std::endl;
		// std::cout << "===========================================================================" << std::endl;
}

void	Request::fileUpload()
{
	string cont_length = _request["Content-Length:"];

	size_t	pos3 = this->_buffCopy.find("filename=");
	std::string fileName;
	if (pos3 != std::string::npos)
	{
		std::string str = this->_buffCopy.substr(pos3 + 1, ft::from_string<int>(cont_length));
		size_t	pos = str.find("\"");
		if (pos != std::string::npos)
		{
			str = str.substr(pos, ft::from_string<int>(cont_length));
			str = str.substr(str.find("\"") + 1, ft::from_string<int>(cont_length));
			str = str.substr(0, str.find("\""));
			fileName = str;
		}
	}
	else
	{
		size_t	pos = this->_reqUrl.find_last_of('/');
		fileName = this->_reqUrl.substr(pos + 1, this->_reqUrl.length());
	}
	mkdir("webservfileupload", 0777);
	std::ofstream	outfile(("webservfileupload/" + fileName).c_str());
	std::string	body = _buffCopy.substr(this->getHeaderLength() + 4, ft::from_string<int>(cont_length));
	std::string::size_type pos1 = body.find("\r\n\r\n");
	body = body.substr(pos1 + 4, ft::from_string<int>(cont_length));
	if (!outfile.good())
		this->_putCode = "201";
	outfile << body;
}

void	Request::addBodyToQuery()
{
	bool	flag = false;
	std::stringstream str1(this->_postBody);
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
	}	
}

void	Request::parsePostBody()
{
	if (this->_type != POST)
		return ;
    std::string::size_type pos = 0;
	std::string	req = _buffCopy;
	if ((pos = _buffCopy.find("\r\n\r\n")) != std::string::npos 
			&& _postFlag == false && this->_flagEnc == false && this->_isFileUpload == false)
	{
		std::string cont_length;
		if (_request["content-length:"].empty() == false)
        	cont_length = _request["content-length:"];
		else if (_request["Content-Length:"].empty() == false)
        	cont_length = _request["Content-Length:"];
		else
			return ;
		std::string	body = req.substr(pos, ft::from_string<int>(cont_length));
		std::string::size_type pos1 = 0;
		while ((pos1 = body.find("\r\n", pos1)) != std::string::npos)
		{
			body.replace(pos1, 2, "\n");
			pos1 += 1; // Move past the replaced '\n' to avoid an infinite loop
		}
		_postBody = body;
		// std::cout << "===========================================================================" << std::endl;
		// std::cout << "{" << body << "}" << std::endl;
		// std::cout << "===========================================================================" << std::endl;
		_postFlag = true;
	}
	if (this->isUrlCgi() == true)
	{
		addBodyToQuery();
		return ;
	}
	if ((pos = _buffCopy.find("\r\n\r\n")) != std::string::npos && _request["Transfer-Encoding:"] == "chunked")
    	parseChunkedBody();
	if ((pos = _buffCopy.find("\r\n\r\n")) != std::string::npos && this->_isFileUpload)
    	fileUpload();
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
	std::size_t pos = 0;
	bool	flag = false;
	pos = this->_reqUrl.find_first_of('?');
	if (!pos)
		return;
	std::string str;
	if (pos != std::string::npos)
		str = this->_reqUrl.substr(pos + 1, (this->_reqUrl.length() - pos));
	this->_queryString = str;
	this->queryURl = this->_reqUrl;
	this->_reqUrl = this->_reqUrl.substr(0, pos);

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
	}	
}

void	Request::headerValidation()
{
	if (_type == DEFAULT)
		throw std::runtime_error("400");
	if (_type == POST)
	{
		std::map<std::string, std::string>::iterator it;
		bool	flagClen = false;
		for (it = this->_request.begin(); it != _request.end(); ++it)
		{
			if (it->first == "content-length:")
				flagClen = true;
			if (it->first == "Content-Length:")
				flagClen = true;
			if (it->first == "Transfer-Encoding:")
				this->_flagEnc = true;
		}
		if (flagClen == this->_flagEnc)
			throw std::runtime_error("400");
	}
}

void	Request::setReqType(const std::string &key)
{
	if (key.find("GET") != std::string::npos)
		this->_type = GET;
	else if (key.find("POST") != std::string::npos)
		this->_type = POST;
	else if (key.find("DELETE") != std::string::npos)
		this->_type = DELETE;
	else if (key.find("PUT") != std::string::npos)
		this->_type = PUT;
	else if (key.find("HEAD") != std::string::npos)
		this->_type = HEAD;
}

void    Request::parseRequest(bool	flag)
{

	this->_buffCopy = _buff;
	// std::string::size_type pos = 0;
	// while ((pos = _buff.find("\r\n", pos)) != std::string::npos)
	// {
	// 	_buff.replace(pos, 2, "\n");
	// 	pos += 1;
	// }
	std::stringstream str(_buff);
    std::string       str1;
    std::string       key;
    std::string       value;
    bool              first = true;

    while (getline(str, str1, '\n'))
    {
		if ((str1.empty() || isWhiteSpace(str1)) && _type == GET)
			continue;
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
			setReqType(key);
			std::size_t	pos_idx = _reqUrl.find("/cgi-bin");
			if (pos_idx != std::string::npos)
				this->_isUrlCgi = true;
            first = false;
		}
		if (key == "Host:")
			this->_host = value;
		if (key == "Content-Length:" || key == "content-length:")
			this->_contLen = ft::from_string<int>(value);
		if (key == "Content-Type:" && value.find("multipart/form-data") != std::string::npos)
			this->_isFileUpload = true;
    }
	if (flag)
	{
		std::cout << "===========================================================================" << std::endl;
		std::cout << _buffCopy << std::endl;
		std::cout << "===========================================================================" << std::endl;
		headerValidation();
		parsePostBody();
	}
	if (getReqType() == DELETE)
		setDeleteURL();
}

std::string	Request::getDeleteURL() const
{
	return (this->deleteURl);
}

void	Request::setDeleteURL()
{
	std::size_t pos;

	pos = this->getReqUrl().find_last_of("/");
	if (pos != std::string::npos)
		this->deleteURl = this->getReqUrl().substr(pos + 1, this->getReqUrl().length());
}

int	Request::getHeaderLength()
{
	size_t		pos;
	std::string	header;
	int			headerLen;
	if ((pos = _buffCopy.find("\r\n\r\n")) != std::string::npos)
	{
		header = this->_buffCopy.substr(0, pos);
	}
	headerLen = header.length();
	return (headerLen);
}

std::map<std::string, std::string>	Request::parseUnderScore()
{
	std::map<std::string, std::string> mapCopy = _request;
	std::map<std::string, std::string> mapC;
	std::map<std::string, std::string>::iterator it;
	for (it = mapCopy.begin(); it != mapCopy.end(); ++it)
	{
		std::string key = replaceChar(it->first);
		key = replaceotherChar(it->first);
		key = strToUpper(key);
        key = "HTTP_" + key;
		key = ft::string(key).replace_all("-", "_");
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

std::string		Request::getStrRequestType() const
{
	if (this->_type == GET)
		return ("GET");
	if (this->_type == POST)
		return ("POST");
	if (this->_type == DELETE)
		return ("DELETE");
	if (this->_type == PUT)
		return ("PUT");
	if (this->_type == HEAD)
		return ("HEAD");
	return ("DEFAULT");
}


std::map<std::string, std::string> Request::getRequest() const
{
	return (_request);
}

std::string Request::getReqUrl() const
{
	return ft::string(_reqUrl).replace_all(" ", "%20");
}

std::string	Request::getQueryString() const
{
	return (this->_queryString);
}

std::string Request::getPostBody() const
{
	return _postBody;
}

std::string	Request::getQueryUrl() const
{
	return (queryURl);
}

std::string	Request::getHost() const
{
	return ft::string(_host).replace_all(" ", "%20");
}

e_request_type	Request::getReqType() const
{
	return (_type);
}

size_t	Request::getContLen()
{
	return(_contLen);
}

bool Request::isUrlCgi() const
{
	return _isUrlCgi;
}

std::string	Request::getPutCode() const
{
	return (this->_putCode);
}

std::string Request::getCgiUrl() const{

    std::string path = this->getReqUrl();
    std::string ret;

    if (path.find_first_of('?') != std::string::npos)
    {
        ret = path.substr(0, path.find_first_of('?'));
        return (ret);
    }
    return(path);
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

std::string Request::replaceotherChar(std::string str)
{
	for (size_t pos = str.find(':'); pos != std::string::npos; pos = str.find(':'))
	{
		str.replace(pos, 1, "");
	}
	return(str);
}