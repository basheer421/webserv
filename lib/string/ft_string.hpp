/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 23:18:20 by bammar            #+#    #+#             */
/*   Updated: 2023/07/22 23:22:11 by bammar           ###   ########.fr       */
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
			std::vector<ft::string> split(char sep);
			string& operator=(const std::string& other);
	};
}
