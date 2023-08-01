/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 23:19:23 by bammar            #+#    #+#             */
/*   Updated: 2023/08/01 15:47:12 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.hpp"

namespace ft
{
	string::string() : std::string() {}
	string::string(const char* str) : std::string(str) {}
	string::string(const std::string& str) : std::string(str) {}
	string& string::operator=(const std::string& other)
	{
		std::string::operator=(other);
		return *this;
	}

	std::vector<ft::string> string::split(char sep)
	{
		std::vector<ft::string> vec;
		std::stringstream ss(*this);
		ft::string buff;

		while (std::getline(ss, buff, sep))
		{
			if (buff.length() == 0)
				continue ;
			vec.push_back(buff);
		}
		return vec;
	}

	string string::rstrip(char c)
	{
		string cpy(*this);

		while (*(cpy.end() - 1) == c)
			cpy.erase(cpy.end() - 1);
		return (cpy);
	}

	string string::lstrip(char c)
	{
		string cpy(*this);

		size_t spos = 0;
		for (spos = 0; spos < cpy.size() && cpy[spos] == c; ++spos)
			;
		return cpy.substr(spos);
	}

	string string::strip(char c)
	{
		return lstrip(c).rstrip(c);
	}

	string file_to_string(const string& file_name)
	{
		std::stringstream ss;
		std::ifstream file;

		file.exceptions ( std::ifstream::badbit );
		file.open(file_name.c_str());
		ss << file.rdbuf();
		if (ss.bad())
			throw std::exception();
		file.close();
		return (ss.str());
	}
}


