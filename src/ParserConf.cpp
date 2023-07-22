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
std::vector<ParserConf::Module> ParserConf::parseFile()
{
	std::vector<ParserConf::Module> file;

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
			Module modl;

			modl.name = line.substr(1, line.length() - 2);
			file.push_back(modl);
		}
		else
		{
			std::vector<ft::string> segments = line.split(' ');
			Directive dir;

			if (segments.empty())
				continue ;
			/*
				First element should always be a Module.
			*/
			if (file.empty())
				throw std::exception();
			
			/*
				There should be a name at the start.
			*/
			if (segments.size() == 1)
				throw std::exception();
			
			dir.first = segments[0];
			segments.erase(segments.begin());
			dir.second = segments;
			
			file[file.size() - 1].directives.push_back(dir);
		}
	}
	return (file);
}

void ParserConf::printDirective(const Directive& dir)
{
	std::cout << dir.first << " ";
	for (std::vector<ft::string>::const_iterator dit2 =
		dir.second.begin();
		dit2 != dir.second.end();
		++dit2)
	{
		std::cout << "{" << *dit2  << "}" << (dit2 + 1 == dir.second.end() ? "" : " ");
	}
	std::cout << "\n";
}

void ParserConf::print(const std::vector<ParserConf::Module>& conf)
{
	for (std::vector<ParserConf::Module>::const_iterator it = conf.begin();
		it != conf.end(); ++it)
	{
		std::cout << "[" << (*it).name << "]\n";

		for (std::vector<Directive>::const_iterator dit =
			(*it).directives.begin();
			dit != (*it).directives.end();
			++dit)
		{
			printDirective(*dit);
		}
	}
}
