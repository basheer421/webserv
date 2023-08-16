/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 22:18:03 by bammar            #+#    #+#             */
/*   Updated: 2023/08/16 12:38:57 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ParserConf.hpp"
#include "webserv.hpp"
#include "Request.hpp"
#include "Pages.hpp"
# include <sys/stat.h>

class Server
{

private:
	struct sockaddr_in address;
	std::list<int> clients;
	int addrlen;
	int server_fd;
	std::vector<ServerTraits> conf;

	class ServerException : public std::exception
	{
		public:
			const ft::string& msg;
			ServerException(const ft::string& msg);
			virtual const char *what() const throw();
	};

	// There shouldn't be a default constructor
	Server();

public:
	
	Server(const std::vector<ServerTraits>& cnf);
	Server(const Server& src);
	Server& operator = (const Server& src);
	~Server();

	void run(); // Throws
	int	is_dir(const char *path);
	void sendResponse(const int& client, Request& request);
};
