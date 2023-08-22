/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 15:03:49 by mkhan             #+#    #+#             */
/*   Updated: 2023/08/22 13:35:28 by mkhan            ###   ########.fr       */
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

void	Response::setResponseHeader(std::string code, std::string mssg)
{
	this->setCode(code);
	this->setMssg(mssg);
	this->setHeader();
}

// if the response doesn't have body content-lenght is zero
void	Response::setHeader()
{
	this->header.clear();
	this->header += "HTTP/1.1 " + code + " " + mssg + CRLF
					"Content-Type: " + content_type + "; charset=utf-8" CRLF;
}

std::string	Response::getResponse()
{
	this->res.clear();
	this->res += this->header;
	this->res += this->res_body;
	return (res);
}	

// check for file access.
void	Response::setBody(std::string path)
{
	std::string body;

	if (is_dir(path.c_str()))
		body = dirList(path);
	else
		body = ft::file_to_string(path);

	this->res_body.clear();
	this->res_body = body;
	this->content_len = res_body.length();
	this->header += "Content-Length: " + ft::to_string(this->content_len) + CRLF
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

Response::~Response()
{
}

int	Response::is_dir(const char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) != 0)
		return (0);
	return (S_ISDIR(statbuf.st_mode));
}