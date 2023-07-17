/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserConf.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 11:19:54 by bammar            #+#    #+#             */
/*   Updated: 2023/07/16 19:18:06 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <utility>
#include <vector>
#include <string>

/**
 * Doesn't handle comments for now
*/
class ParserConf
{
	private:
		std::string text;
		std::string::iterator iter;	
	public:
		typedef std::pair<std::string, std::vector<std::string> > Directive;
		struct Module
		{
			std::string name;
			std::vector<Directive> directives;
			std::vector<Module> modules;
		};
		ParserConf();
		ParserConf(std::string text);
		ParserConf(const ParserConf& src);
		ParserConf& operator = (const ParserConf& src);
		~ParserConf();
		std::string parseWord();
		Directive parseDirective();
		Module parseModule();
		std::vector<Module> parseFile();
};
