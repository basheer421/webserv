/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 22:18:03 by bammar            #+#    #+#             */
/*   Updated: 2023/08/22 12:18:10 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

class Server
{

private:
	struct sockaddr_in address;
	std::list<int> clients;
	int addrlen;
	int serverFd;
	ServerTraits conf;
	std::map<std::string, std::string> envMap;

	// There shouldn't be a default constructor
	Server();

public:

	Server(const ServerTraits& cnf);
	Server(const Server& src);
	Server& operator = (const Server& src);
	~Server();

	void run(); // Throws
	static int	is_dir(const char *path);

	int getServerFd() const;
	struct sockaddr *getAddress() const;
	socklen_t *getAddrlen() const;
	const ServerTraits& getConf() const;
	
	void	parseEnv(char **rawEnv);
	std::map<std::string, std::string> getEnv() const;
	std::string	strToUpper(std::string str);
};
