/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 22:18:03 by bammar            #+#    #+#             */
/*   Updated: 2023/08/01 15:03:31 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"
#include "Request.hpp"

class Server
{

private:
	struct sockaddr_in address;
	std::list<int> clients;
	int addrlen;
	int server_fd;
	const std::map<ft::string, ParserConf::Module> conf;

	class ServerException : public std::exception
	{
		public:
			const ft::string& msg;
			ServerException(const ft::string& msg);
			virtual const char *what() const throw();
	};

public:
	Server(); // Throws
	Server(const std::map<ft::string, ParserConf::Module>& cnf);
	Server(const Server& src);
	Server& operator = (const Server& src);
	~Server();

	void run(); // Throws
	void sendResponse(const int& client, Request& request);
};
