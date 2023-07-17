/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 20:06:07 by bammar            #+#    #+#             */
/*   Updated: 2023/07/17 20:09:31 by bammar           ###   ########.fr       */
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
	ParserConf::print(conf);

	(void)conf;
	file.close();
    return (0);
}
