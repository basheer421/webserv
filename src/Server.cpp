/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 22:26:15 by bammar            #+#    #+#             */
/*   Updated: 2023/07/26 22:51:20 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::ServerException::ServerException(const ft::string& msg) : msg(msg) {}

const char *Server::ServerException::what() const throw()
{
	return (msg.c_str());
}

Server::Server()
{
	addrlen = sizeof(address);
	bzero((char *)&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(PORT);
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
		throw ServerException("Socket Error");
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		throw ServerException("Bind Error");
    if (listen(server_fd, 10) < 0)
		throw ServerException("Listen Error");
}

Server::Server(const Server& src)
{
	if (this == &src)
		return ;
	*this = src;
}

Server& Server::operator = (const Server& src)
{
	if (this == &src)
		return *this;
	this->address = src.address;
	this->addrlen = src.addrlen;
	this->server_fd = src.server_fd;
	return *this;
}

void Server::run()
{
	while (true)
    {
        int client;
		// struct pollfd pfds[FD_COUNT];

		std::cout << "Waiting for connections on PORT: " << PORT << "\n";
		// pfds[0] = (struct pollfd) {server_fd, POLLIN, 0};
		
		// int index = 0;
        // for (std::list<int>::iterator it = clients.begin(); it != clients.end(); ++it)
		// 	pfds[++index] = (struct pollfd) {*it, POLL_IN, 0};
        
		// if (poll(pfds, clients.size() + 1, -1) < 0)
		// 	throw ServerException("Poll Error");
		client = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (client < 0)
			throw ServerException("AcceptException");
        char buffer[30000] = {0};

        int read_res = read(client, buffer, 29999);
        if (read_res < 0)
			throw ServerException("ReadException");
        std::string buffer_str = buffer;
        std::cout << buffer_str << std::endl;

		// Parse request here

		// Sending 404 to test server
		ft::string res_body(
			"<html>" CRLF
			"<head><title>404 Not Found</title></head>" CRLF
			"<body>" CRLF
			"<h1 style='text-align:center; marign:2rem; margin-top:3rem;'>404 Not Found</h1>" CRLF
			"<hr>" CRLF
			"<h2 style='text-align:center;'>webserv</h2>" CRLF
			"</body>" CRLF
			"</html>" CRLF
		);

		ft::string response (
			"HTTP/1.1 404 Not Found" CRLF
			"Content-Type: text/html; charset=utf-8" CRLF
			"Content-Length: 210" CRLF
			CRLF
		);
		response += res_body;
		send(client, response.c_str(), response.length(), 0);

        close(client);
    }
}

Server::~Server() {}
