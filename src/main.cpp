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

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;

	try
	{
		// reading the file and storing in text
		// ! Restructure this part here
		std::fstream file("conf.ini", std::fstream::in);
		std::stringstream textStream;
		textStream << file.rdbuf();
		file.close();
		ft::string text = textStream.str();

		// Parsing for the configuration file
		ParserConf parser(text);
		std::vector<ServerTraits> conf = parser.parseFile();

		// Starting the server here
		ServerManager serverManager(conf);
		serverManager.run(envp);

	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
    return (0);
}
