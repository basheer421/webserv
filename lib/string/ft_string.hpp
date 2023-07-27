/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 23:18:20 by bammar            #+#    #+#             */
/*   Updated: 2023/07/23 01:11:06 by bammar           ###   ########.fr       */
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
}
