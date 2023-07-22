/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserConf.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 11:19:54 by bammar            #+#    #+#             */
/*   Updated: 2023/07/22 19:37:27 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

class ParserConf
{
	public:
		typedef std::pair<std::string, std::vector<std::string> > Directive;	
		struct Module
		{
			std::string name;
			std::vector<Directive> directives;
		};

	private:	
		
		std::string text;
		std::string::iterator iter;

		static bool isModuleName(std::string& str);
		static void printDirective(const Directive& dir);
		static void removeComment(std::string& str);
		static void replaceSpaces(std::string& str);
		static void strip(std::string& str, char c);

	public:
		ParserConf();
		ParserConf(std::string& text);
		ParserConf(const ParserConf& src);
		ParserConf& operator = (const ParserConf& src);
		~ParserConf();

		
		std::vector<Module> parseFile();

		static void print(const std::vector<ParserConf::Module>& conf);
};
