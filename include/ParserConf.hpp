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

#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include "ft_string.hpp"
#include "ServerTraits.hpp"

class ParserConf
{
	public:
		typedef std::vector<ft::string> Directive;
		typedef std::map<ft::string, Directive> Module;

		class ParseException : public std::exception
		{
			public:
				const ft::string& msg;
				ParseException(const ft::string& msg);
				virtual const char *what() const throw();
		};

	private:
		ft::string text;
		ft::string::iterator iter;
		std::map< ft::string, std::vector<ParserConf::Module> > conf;

		static bool isModuleName(ft::string& str);
		static void removeComment(ft::string& str);
		static void replaceSpaces(ft::string& str);

	public:
		ParserConf();
		ParserConf(ft::string& text);
		ParserConf(const ParserConf& src);
		ParserConf& operator = (const ParserConf& src);
		~ParserConf();

		std::map< ft::string, std::vector<ParserConf::Module> > parseFile();
		std::vector<ServerTraits> parseToStruct();
		void setAddress(ft::string& confAdrss, in_addr_t &address, in_port_t& port);
};
