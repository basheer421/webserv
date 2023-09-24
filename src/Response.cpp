/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 15:03:49 by mkhan             #+#    #+#             */
/*   Updated: 2023/09/24 21:16:51 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response()
{
	this->header = "HTTP/1.1";
	this->code = "200";
	this->mssg = "OK";
	this->content_type = "text/html";
	this->content_len = 0;
	this->res_body = "";
	this->res = "";
	setResponseHeader(this->code, this->mssg);
	parseMimes();
}

Response::Response(Response const &object)
{
	*this = object;
}

Response &Response::operator=(Response const &rhs)
{
	if (this != &rhs)
	{
		//
	}
	return (*this);
}

void	Response::parseMimes()
{
	std::ifstream	mimieFile("mimes.txt");
	std::string		line;
	
	if (mimieFile.fail())
	{
		std::cout << "Error opening file" << std::endl;
		mimieFile.close();
	}
	getline(mimieFile, line);
	while (!(mimieFile.eof()))
	{
		std::stringstream	str(line);
		std::string			ext, type;
		getline(str, ext, ' ');
		getline(str, type);
		this->mimes[ext] = type;
		getline(mimieFile, line);
	}
	mimieFile.close();
}

void	Response::setResponseHeader(std::string code, std::string mssg)
{
	this->setCode(code);
	this->setMssg(mssg);
	this->setHeader();
}

// if the response doesn't have body content-length is zero
void	Response::setHeader()
{
	this->header.clear();
	this->header += "HTTP/1.1 " + code + " " + mssg + CRLF;
}

std::string	Response::getResponse()
{
	this->res.clear();
	this->res += this->header;
	this->res += this->res_body;
	return (res);
}	

std::string Response::getbody()
{
	return (res_body);
}

// check for file access.
void	Response::setBody(const std::string& path, const Request &request, bool autoindex)
{
	std::string body;
	std::string type;
	size_t pos;

		std::cout << "*****************" << std::endl;
		std::cout << path << std::endl;
		std::cout << "*****************" << std::endl;
	if (request.getReqType() != POST && request.getReqType() != PUT)
	{
		if (is_dir(path.c_str()) && autoindex)
			body = dirList(path, request.getReqUrl());
		else if (is_dir(path.c_str()) && request.getReqType() ==  DELETE)
			body = dirList(path, request.getReqUrl());
		else if (is_file(path.c_str()))
			body = ft::file_to_string(path);
		else
			throw ("404");
	}

	std::map<std::string, std::string>::iterator it;
	for (it = this->mimes.begin(); it != this->mimes.end(); ++it)
	{
		if((pos = path.find_last_of('.')) != std::string::npos)
			type = path.substr(pos, path.length() - pos);
		if (it->first == type)
			this->content_type = it->second;			 
	}
	this->res_body.clear();
	this->res_body = body;
	this->content_len = res_body.length();
	if (request.getReqType() == POST || request.getReqType() == PUT)
	{
		if (request.getPutCode() == "201")
			this->setResponseHeader("201", "Created");
		if (request.getReqType() == POST)
			this->setResponseHeader("200", "OK");
		this->res_body.clear();
	}
	if (request.getReqType() == HEAD)
		this->res_body.clear();
	if (request.getReqType() == DELETE)
	{
		if (remove(request.getDeleteURL().c_str()) != 0)
			throw std::runtime_error("400");
	}
	this->header += "Content-Type: " + content_type + "; charset=utf-8" CRLF
					"Content-Length: " + ft::to_string(this->content_len) + CRLF
					CRLF;
}

void	Response::setCgiBody(std::string body)
{
	this->res_body.clear();
	this->res_body = body;
	this->content_len = res_body.length();
	this->header += "Content-Type: " + content_type + "; charset=utf-8" CRLF
					"Content-Length: " + ft::to_string(this->content_len) + CRLF
					CRLF;
}

void	Response::setErrBody(std::string body, const Request &request)
{
	this->res_body.clear();
	this->res_body = body;
	this->content_len = res_body.length();
	if (request.getReqType() == HEAD)
		this->res_body.clear();
	this->header += "Content-Type: " + content_type + "; charset=utf-8" CRLF
					"Content-Length: " + ft::to_string(this->content_len) + CRLF
					CRLF;
}

void	Response::setCode(std::string str)
{
	this->code = str;
}

void	Response::setMssg(std::string str)
{
	this->mssg = str;
}

void	Response::setCtype(std::string str)
{
	this->content_type = str;
}

void	Response::setClen(int len)
{
	this->content_len = len;
}

std::string	Response::getHeader()
{
	return (this->header);
}

// ِAppends a string to the header including CRLF
void Response::appendHeader(const std::string& str)
{
	this->header += str + CRLF;
}

Response::~Response()
{
}

int	is_dir(const string path)
{
	struct stat	statbuf;

	if (stat(path.c_str(), &statbuf) != 0)
		return (0);
	return (S_ISDIR(statbuf.st_mode));
}

int is_file(const string path)
{
	struct stat	statbuf;

	if (stat(path.c_str(), &statbuf) != 0)
		return (0);
	return (S_ISREG(statbuf.st_mode));
}
