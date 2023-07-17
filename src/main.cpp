/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 20:06:07 by bammar            #+#    #+#             */
/*   Updated: 2023/07/17 13:35:49 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include <fstream>
#include <sstream>

int main(void)
{
    // Server server;
	// server.run();
	std::fstream file("conf", std::fstream::in);
	std::stringstream textStream;
	textStream << file.rdbuf();
	std::string text = textStream.str();
	
	ParserConf parser(text);
	std::vector<ParserConf::Module> conf = parser.parseFile();
	// std::cout << "modls count: " << conf.size() << "\n";
	// for (size_t i = 0; i < conf.size(); i++)
	// {
	// 	std::vector<ParserConf::Directive> x = conf[i].directives;
	// 	std::cout << conf[i].name << "[\n";
	// 	for (std::vector<ParserConf::Directive>::iterator it = x.begin(); it != x.end(); it++)
	// 	{
	// 		ParserConf::Directive dir = *it;
	// 		std::cout << dir.first  << " ";
	// 		for (std::vector<std::string>::iterator it = dir.second.begin(); it != dir.second.end(); it++)
	// 		{
	// 			;// std::cout << *it << " ";
	// 		}
	// 		std::cout << "\n";
	// 	}
	// 	std::cout << conf[i].name << "]\n";
	// }
	(void)conf;
	file.close();
    return (0);
}
