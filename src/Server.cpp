/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 22:26:15 by bammar            #+#    #+#             */
/*   Updated: 2023/07/13 23:23:22 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

const char *Server::SocketException::what() const throw()
{
	return ("Bad socket");
}

const char *Server::BindException::what() const throw()
{
	return ("Bind failed");
}

const char *Server::ListenException::what() const throw()
{
	return ("Listen failed");
}

const char *Server::AcceptException::what() const throw()
{
	return ("Accept failed");
}

const char *Server::ReadException::what() const throw()
{
	return ("Read failed");
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
		throw SocketException();
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		throw BindException();
    if (listen(server_fd, 10) < 0)
		throw ListenException();
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

        std::cout << "Waiting for a connection on PORT: " << PORT << "\n";
        client = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (client < 0)
			throw AcceptException();
        char buffer[30000] = {0};
        int read_res = read(client, buffer, 29999);
        if (read_res < 0)
			throw ReadException();
        std::string buffer_str = buffer;
        std::cout << buffer_str << std::endl;
		Request req(buffer);
		req.parseRequest();
        close(client);
    }
}

Server::~Server() {}
