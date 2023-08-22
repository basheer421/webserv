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
	std::fstream file("conf.ini", std::fstream::in);
	std::stringstream textStream;
	textStream << file.rdbuf();
	file.close();
	ft::string text = textStream.str();
	ParserConf parser(text);
	std::vector<ServerTraits> conf = parser.parseFile();
	ServerManager serverManager(conf);
	serverManager.run();
    return (0);
}
