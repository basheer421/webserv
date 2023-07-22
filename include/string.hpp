/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 22:52:02 by bammar            #+#    #+#             */
/*   Updated: 2023/07/22 23:13:53 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <fstream>

namespace ft
{
	/**
	 * std::string overload to apply some of the useful methods.
	*/
	class string : public std::string
	{
		public:
			string() : std::string() {}
        	string(const char* str) : std::string(str) {}
        	string(const std::string& str) : std::string(str) {}

			std::vector<ft::string> split(char sep)
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

			string& operator=(const std::string& other)
			{
				std::string::operator=(other);
				return *this;
			}
	};
}
