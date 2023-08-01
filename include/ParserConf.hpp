/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserConf.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 11:19:54 by bammar            #+#    #+#             */
/*   Updated: 2023/07/26 16:16:11 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <utility>
#include <vector>
#include <map>
#include "ft_string.hpp"
#include <iostream>

class ParserConf
{
	public:
		typedef std::vector<ft::string> Directive;
		struct Module
		{
			std::map<ft::string, Directive> directives;
		};

	private:	
		
		ft::string text;
		ft::string::iterator iter;

		static bool isModuleName(ft::string& str);
		static void printDirective(const Directive& dir);
		static void removeComment(ft::string& str);
		static void replaceSpaces(ft::string& str);

	public:
		ParserConf();
		ParserConf(ft::string& text);
		ParserConf(const ParserConf& src);
		ParserConf& operator = (const ParserConf& src);
		~ParserConf();

		std::map<ft::string, Module> parseFile();

		static void print(const std::map<ft::string, ParserConf::Module>& conf);
};
