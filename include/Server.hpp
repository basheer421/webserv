/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 22:18:03 by bammar            #+#    #+#             */
/*   Updated: 2023/08/04 17:27:52 by bammar           ###   ########.fr       */
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
	ft::string conf_addrs;

	class ServerException : public std::exception
	{
		public:
			const ft::string& msg;
			ServerException(const ft::string& msg);
			virtual const char *what() const throw();
	};
	void addressInit();
	void setAddress();

public:
	Server(); // Throws
	Server(const std::map<ft::string, ParserConf::Module>& cnf);
	Server(const Server& src);
	Server& operator = (const Server& src);
	~Server();

	void run(); // Throws
	void sendResponse(const int& client, Request& request);
};
