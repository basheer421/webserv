/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 23:18:20 by bammar            #+#    #+#             */
/*   Updated: 2023/07/31 19:41:54 by bammar           ###   ########.fr       */
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
			string();
        	string(const char* str);
        	string(const std::string& str);
			string& operator=(const std::string& other);

			std::vector<string> split(char sep);

			string rstrip(char c);
			string lstrip(char c);
			string strip(char c);

	};

	template <typename T>
	string to_string(T n)
	{
		std::stringstream ss;

		ss << n;
		return (ss.str());
	}

	template <typename T>
	T from_string(string str)
	{
		std::stringstream ss(str);

		T n;
		ss >> n;
		return (n);
	}
}
