/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserConf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 12:05:54 by bammar            #+#    #+#             */
/*   Updated: 2023/07/22 19:37:43 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParserConf.hpp"

ParserConf::ParserConf() {}

void ParserConf::strip(std::string& str, char c)
{
	while (*(str.end() - 1) == c)
		str.pop_back();
	size_t spos = 0;
	for (spos = 0; spos < str.size() && str[spos] == c; ++spos)
		;
	str = str.substr(spos);
}

// Replaces all spaces with ' ' except '\\n'
void ParserConf::replaceSpaces(std::string& str)
{
	for (size_t i = 0; i < str.size(); ++i)
	{
		if (std::isspace(str[i]) && str[i] != '\n')
			str[i] = ' ';
	}
}

// Sets the end of the str to be ';' exclusive
void ParserConf::removeComment(std::string& str)
{
	str = str.substr(0, str.find(';'));
}

ParserConf::ParserConf(std::string& text) : text(text)
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

bool ParserConf::isModuleName(std::string& str)
{
	return (*(str.begin()) == '[' && *(str.end() - 1) == ']');
}

static std::vector<std::string> split(std::string& str, char sep)
{
	std::vector<std::string> vec;
	std::stringstream ss(str);
	std::string buff;

	while (std::getline(ss, buff, sep))
	{
		if (buff.length() == 0)
			continue ;
		vec.push_back(buff);
	}
	return vec;
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

	std::vector<std::string> lines = split(text, '\n');

	for (std::vector<std::string>::iterator it = lines.begin();
		it < lines.end(); ++it)
	{
		std::string& line = *it;
		removeComment(line);
		replaceSpaces(line);
		strip(line, ' ');

		if (isModuleName(line))
		{
			Module modl;

			modl.name = line.substr(1, line.length() - 2);
			file.push_back(modl);
		}
		else
		{
			std::vector<std::string> segments = split(line, ' ');
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
	for (std::vector<std::string>::const_iterator dit2 =
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
