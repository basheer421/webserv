/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserConf.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 11:19:54 by bammar            #+#    #+#             */
/*   Updated: 2023/07/21 18:12:55 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <utility>
#include <vector>
#include <string>
#include <unistd.h>

/**
 * Doesn't handle comments for now
*/
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
		Module parseModule();
		static bool isModuleName(std::string& str);
		static void printDirective(const Directive& dir);
	public:
		ParserConf();
		ParserConf(std::string& text);
		ParserConf(const ParserConf& src);
		ParserConf& operator = (const ParserConf& src);
		~ParserConf();
		
		std::vector<Module> parseFile();

		static void print(const std::vector<ParserConf::Module>& conf);
};
