/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 20:06:07 by bammar            #+#    #+#             */
/*   Updated: 2023/07/22 23:07:34 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main(void)
{
    // Server server;
	// server.run();
	std::fstream file("conf", std::fstream::in);
	std::stringstream textStream;
	textStream << file.rdbuf();
	ft::string text = textStream.str();
	
	ParserConf parser(text);
	std::vector<ParserConf::Module> conf = parser.parseFile();
	ParserConf::print(conf);

	(void)conf;
	file.close();
    return (0);
}
