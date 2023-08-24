/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 22:26:15 by bammar            #+#    #+#             */
/*   Updated: 2023/08/24 13:26:49 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(const ServerTraits& cnf) : conf(cnf)
{
	int optval;

	optval = 1;

	std::memset(&address, 0, sizeof(address));
	address.sin_addr.s_addr = conf.listen_address;
	address.sin_port = conf.listen_port;
	address.sin_family = AF_INET;
	// address.sin_len = sizeof(address);

	serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverFd < 0)
		throw std::runtime_error("Socket Error");
	if (fcntl(serverFd, F_SETFL, O_NONBLOCK, FD_CLOEXEC) < 0)
		throw std::runtime_error("fcntl Error"); 
	if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
		throw std::runtime_error("setsockopt Error");
	if (bind(serverFd, (struct sockaddr *)&address, sizeof(address)) < 0)
		throw std::runtime_error("Bind Error");
    if (listen(serverFd, 10) < 0)
		throw std::runtime_error("Listen Error");
}

Server::Server(const Server& src)
{
	if (this == &src)
		return ;
	*this = src;
	conf = src.conf;
}

Server& Server::operator = (const Server& src)
{
	if (this == &src)
		return *this;
	this->address = src.address;
	this->addrlen = src.addrlen;
	this->serverFd = src.serverFd;
	this->conf = src.conf;
	return *this;
}

int Server::getServerFd() const { return serverFd; }

struct sockaddr *Server::getAddress() const { return  ((struct sockaddr *)&address); }

socklen_t *Server::getAddrlen() const { return ((socklen_t*)&addrlen); }

const ServerTraits& Server::getConf() const { return (conf); }

Server::~Server() {}
