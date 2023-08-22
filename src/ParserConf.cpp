/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserConf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 12:05:54 by bammar            #+#    #+#             */
/*   Updated: 2023/07/23 01:13:04 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParserConf.hpp"

ParserConf::ParserConf() {}

ParserConf::ParseException::ParseException(const ft::string& msg) : msg(msg) {}

const char *ParserConf::ParseException::what() const throw()
{
	return (msg.c_str());
}

// Replaces all spaces with ' ' except '\\n'
void ParserConf::replaceSpaces(ft::string& str)
{
	for (size_t i = 0; i < str.size(); ++i)
	{
		if (std::isspace(str[i]) && str[i] != '\n')
			str[i] = ' ';
	}
}

// Sets the end of the str to be ';' exclusive
void ParserConf::removeComment(ft::string& str)
{
	str = str.substr(0, str.find(';'));
}

ParserConf::ParserConf(ft::string& text) : text(text)
{
	iter = this->text.begin();
}

ParserConf::ParserConf(const ParserConf& src)
{
	if (this == &src)
		return ;
	*this = src;
}

ParserConf& ParserConf::operator = (const ParserConf& src)
{
	if (this == &src)
		return *this;
	text = src.text;
	iter = src.iter;
	return *this;
}

ParserConf::~ParserConf() {}

bool ParserConf::isModuleName(ft::string& str)
{
	return (*(str.begin()) == '[' && *(str.end() - 1) == ']');
}

void ParserConf::fillRouteValue(ServerRoute& route, string& name, std::vector<string>& segments)
{
	if (name == "limit_except")
		route.limit_except = segments;
	else if (name == "autoindex")
	{
		if (segments.size() > 1)
			throw ParseException("Parse Error: 'autoindex' should have 1 value");
		if (segments.front() == "true")
			route.autoindex = true;
		else if (segments.front() == "false")
			route.autoindex = false;
		else
			throw ParseException("Parse Error: could not parse 'autoindex'");
	}
	else if (name == "try_files")
		route.try_files = segments;
	else if (name == "return")
	{
		if (segments.size() > 1)
			throw ParseException("Parse Error: 'return' should have 1 value");
		route.return_ = segments.front();
	}
}

void ParserConf::fillServerValue(ServerTraits& server, string& name, std::vector<string>& segments)
{
	if (name == "root")
	{
		if (segments.size() > 1)
			throw ParseException("Parse Error: 'root' should have 1 value");
		server.root = segments.front();
	}
	else if (name == "index")
	{
		server.index = segments;
	}
	else if (name == "listen")
	{
		if (segments.size() > 1)
			throw ParseException("Parse Error: 'listen' should have 1 value");
		setAddress(segments.front(), server.listen_address, server.listen_port);
	}
	else if (name == "server_name")
	{
		server.server_name = segments;
	}
	else if (name == "client_max_body_size")
	{
		if (segments.size() > 1)
			throw ParseException("Parse Error: 'client_max_body_size' should have 1 value");
		try {
			server.client_max_body_size = ft::from_string<size_t>(segments.front()); // Throws
		} catch (std::exception& e) {
			throw ParseException("Parse Error: could not parse 'client_max_body_size'");
		}
	}
}

/**
 * TODO:
 * - Replace all space chars (except '\\n') with '\ ' (space).
 * - Remove everything after ';' (comments).
 * - Make your exceptions.
*/
std::vector<ServerTraits> ParserConf::parseFile()
{
	std::vector<ServerTraits> file;
	ft::string lastModlName;
	bool isRoute;

	std::vector<ft::string> lines = text.split('\n');

	for (std::vector<ft::string>::iterator it = lines.begin();
		it < lines.end(); ++it)
	{
		ft::string& line = *it;
		removeComment(line);
		replaceSpaces(line);
		line = line.strip(' ');

		if (isModuleName(line))
		{
			lastModlName = line.substr(1, line.length() - 2);
			std::vector<string> modlSplit = lastModlName.split(' ');
			if ((modlSplit.size() == 1) && lastModlName == "server")
			{
				file.push_back(ServerTraits());
				isRoute = false;
			}
			else if ((modlSplit.size() == 2) && modlSplit.front() == "location")
			{
				if (modlSplit.back()[0] != '/')
					throw ParseException("Parse Error: Bad module name");

				file.back().routes[modlSplit.back()] = ServerRoute();
				isRoute = true;
			}
			else
				throw ParseException("Parse Error: Bad module name");
		}
		else
		{
			Directive segments = line.split(' ');

			if (segments.empty())
				continue ;
			// First element should always be a Module and there should be a name at the start.
			if (file.empty() || segments.size() == 1)
				throw std::exception();

			ft::string name = segments.front();
			segments.erase(segments.begin());

			if (isRoute)
				fillRouteValue(file.back().routes[lastModlName.split(' ').back()], name, segments);
			else
				fillServerValue(file.back(), name, segments);
		}
	}
	return (file);
}

/**
 * TODO:
 * https://nginx.org/en/docs/http/ngx_http_core_module.html#listen
 * listen address[:port]
 * listen port
 * listen unix:path
 * Default:	listen *:80 | *:8000;
 * 
 * https://stackoverflow.com/questions/15673846
*/
void setAddress(ft::string& confAdrss, in_addr_t &address, in_port_t& port)
{
    std::vector<ft::string> vec = confAdrss.split(':');
    if (vec.size() != 1 && vec.size() != 2)
        throw std::runtime_error("Bad Address: Invalid format");

    // Only port provided
    if (vec.size() == 1)
    {
		address = htonl(INADDR_ANY);
        port = ft::from_string<in_port_t>(vec.at(0));
        if (port <= 0)
            throw std::runtime_error("Bad Address: Invalid port number");
        port = htons(port);
        return;
    }

    // Handling address
    if (vec.at(0) != "*")
    {
		// /**
		//  * This needs to be changed to be what is in the /etc/hosts
		//  * All of the urls there has to be too.
		//  */
		if (vec.at(0) == "localhost")
			vec.at(0) = "127.0.0.1";
        std::vector<ft::string> segments = vec.at(0).split('.');
        if (segments.size() != 4)
            throw std::runtime_error("Bad Address: Invalid IP address format");

        in_addr_t adrs_int = (ft::from_string<in_addr_t>(segments.at(0)) << 24)
                           | (ft::from_string<in_addr_t>(segments.at(1)) << 16)
                           | (ft::from_string<in_addr_t>(segments.at(2)) << 8)
                           | ft::from_string<in_addr_t>(segments.at(3));
        address = htonl(adrs_int);
    }

    // Handling port
    port = ft::from_string<in_port_t>(vec.at(1));
    port = htons(port);
}
