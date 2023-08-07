/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 20:06:07 by bammar            #+#    #+#             */
/*   Updated: 2023/07/23 01:27:22 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main(void)
{
	std::fstream file("conf", std::fstream::in);
	std::stringstream textStream;
	textStream << file.rdbuf();
	file.close();
	ft::string text = textStream.str();
	ParserConf parser(text);
	std::map<ft::string, std::vector<ParserConf::Module> > conf
		= parser.parseFile();
	// Server server(conf);
	// server.run();
    return (0);
}
