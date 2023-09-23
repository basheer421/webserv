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

	if (argc > 2)
		return (1);
	try
	{
		ft::string file;
		// Parsing for the configuration file
		if (argc == 2)
			file = argv[1];
		else
			file = "conf.ini";
		ParserConf parser(file);
		std::vector<ServerTraits> conf = parser.parseFile();

		// Starting the server here // while loop here maybe to keep the server running 
		ServerManager serverManager(conf);
		serverManager.run(envp);
        std::cerr << "EXITED" << std::endl;


	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
    return (0);
}
