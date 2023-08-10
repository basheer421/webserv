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

/**
 * TODO:
 * - Replace all space chars (except '\\n') with '\ ' (space).
 * - Remove everything after ';' (comments).
 * - Make your exceptions.
*/
std::map<ft::string, std::vector<ParserConf::Module> > ParserConf::parseFile()
{
	std::map<ft::string, std::vector<ParserConf::Module> > file;
	ft::string lastModlVec;

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
			lastModlVec = line.substr(1, line.length() - 2);
			file[lastModlVec].push_back(Module());
		}
		else
		{
			Directive segments = line.split(' ');

			if (segments.empty())
				continue ;
			/*
				First element should always be a Module.
				There should be a name at the start.
			*/
			if (file.empty() || segments.size() == 1)
				throw std::exception();

			ft::string name = segments.front();
			segments.erase(segments.begin());
			Module& modl = file[lastModlVec].back();
			modl[name] = segments;
		}
	}
	conf = file;
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
void ParserConf::setAddress(ft::string& confAdrss, in_addr_t &address, in_port_t& port)
{
    std::vector<ft::string> vec = confAdrss.split(':');
    if (vec.size() != 1 && vec.size() != 2)
        throw ParseException("Bad Address: Invalid format");
	else if (vec.size() == 1)
		confAdrss = "*:" + confAdrss;

    // Only port provided
    if (vec.size() == 1)
    {
        port = ft::from_string<in_port_t>(vec.at(0));
        if (port <= 0)
            throw ParseException("Bad Address: Invalid port number");
        port = htons(port);
        return;
    }

    // Handling address
    if (vec.at(0) != "*")
    {
        std::vector<ft::string> segments = vec.at(0).split('.');
        if (segments.size() != 4)
            throw ParseException("Bad Address: Invalid IP address format");

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

std::vector<ServerTraits> ParserConf::parseToStruct()
{
	std::vector<ServerTraits> srvs;

	std::map<ft::string, std::vector<ParserConf::Module> > file = parseFile();

	if (file.empty() ||  (*file.begin()).first != "server")
			return srvs;
	for (std::map<ft::string, std::vector<ParserConf::Module> >::iterator it = file.begin();
		it != file.end(); ++it)
	{
		std::pair<ft::string, std::vector<ParserConf::Module> > modl = *it;
		if (modl.first == "server")
		{
			srvs.push_back(ServerTraits());
			ServerTraits& srv = srvs.back();
			for (size_t i = 0; i < modl.second.size(); ++i)
			{
				ParserConf::Module& modlData = modl.second[i];
				ParserConf::Module::iterator element;

				element = modlData.find("root");
				if (element != modlData.end())
					srvs.back().root = (*element).second[0];
				element = modlData.find("index");
				if (element != modlData.end())
					srvs.back().index = (*element).second;
				element = modlData.find("listen");
				if (element != modlData.end())
					setAddress((*element).second[0], srvs.back().listen_address, srvs.back().listen_port);
				element = modlData.find("server_name");
				if (element != modlData.end())
					srvs.back().server_name = (*element).second;
				element = modlData.find("client_max_body_size");
				if (element != modlData.end())
					srvs.back().client_max_body_size = ft::from_string<size_t>((*element).second[0]);
				// element = modlData.find("routes");
				// if (element != modlData.end())
				// 	srvs.back().routes = (*element).second;
			}
		}
		else if (modl.first.split(' ').front() == "location")
		{
			
		}
		else
			throw std::exception();
	}
}
