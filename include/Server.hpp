/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 22:18:03 by bammar            #+#    #+#             */
/*   Updated: 2023/07/13 22:55:29 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define PORT 8080

#include "webserv.hpp"

class Server
{

private:
	struct sockaddr_in address;
	int addrlen;
	int server_fd;

	class SocketException : public std::exception
	{
		public:
			virtual const char *what() const throw();
	};

	class BindException : public std::exception
	{
		public:
			virtual const char *what() const throw();
	};

	class ListenException : public std::exception
	{
		public:
			virtual const char *what() const throw();
	};

	class AcceptException : public std::exception
	{
		public:
			virtual const char *what() const throw();
	};

	class ReadException : public std::exception
	{
		public:
			virtual const char *what() const throw();
	};

public:
	Server(); // Throws
	Server(const Server& src);
	Server& operator = (const Server& src);
	~Server();

	void run(); // Throws
};
