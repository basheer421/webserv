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
	return (file);
}

void ParserConf::printDirective(const Directive& dir)
{
	for (std::vector<ft::string>::const_iterator dit2 =
		dir.begin();
		dit2 != dir.end();
		++dit2)
	{
		std::cout << "{" << *dit2  << "}" << (dit2 + 1 == dir.end() ? "" : " ");
	}
	std::cout << "\n";
}
