/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 23:19:23 by bammar            #+#    #+#             */
/*   Updated: 2023/07/22 23:27:36 by bammar           ###   ########.fr       */
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
}
