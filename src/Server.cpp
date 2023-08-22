/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 22:26:15 by bammar            #+#    #+#             */
/*   Updated: 2023/08/22 12:29:09 by mkhan            ###   ########.fr       */
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

void Server::run()
{
	// std::cout << "Listening on port: " << htons(conf.listen_port) << "\n";
	// while (true)
	// {
	// 	int client;
		// struct pollfd pfds[FD_COUNT];

		//* do this at the begining instead
		/***************/
		// pfds[0] = (struct pollfd) {serverFd, POLLIN, 0};
		// int index = 0;
		// for (std::list<int>::iterator it = clients.begin(); it != clients.end(); ++it)
		// 	pfds[++index] = (struct pollfd) {*it, POLLIN | POLLOUT, 0};
		/**************/

		//* poll the sockets.array() with sockets.size(), -1 might be changed but -1 for now

		// if (poll(pfds, clients.size() + 1, -1) < 0)
		// 	throw ServerException("Poll Error");


		/*
			instead do a for loop for all servers and if anything is found add it to the end of 'sockets'
		*/

		// if (pfds[0].revents & POLLIN)
		// {
		// 	client = accept(serverFd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
		// 	if (client < 0)
		// 		throw ServerException("AcceptException");
		// 	clients.push_back(client);
		// }

		// We have 1 server so count will be 1 here, otherwise the server count.
		/**
		 * start from 'amount of servers' instead of 1
		 * only
		 */







		// 	index = 1;
		// 	for (std::list<int>::iterator it = clients.begin(); it != clients.end();) {
		// 		int client_fd = *it;

		//* 	"pfds[index] will be sockets[(*it).fd]"

		// 		if (pfds[index].revents & POLLIN) {
		//*		30000 is temp,
		//* 	We should put (client_max_body_size + header size)
		
		// 			char buffer[30000] = {0};
		// 			int read_res = recv(client_fd, buffer, 29999, 0);
		// 			if (read_res <= 0) {
		// 				it = clients.erase(it);
		// 				close(client_fd);
		// 				continue;
		// 			} 
		// 			else
		// 			{
		// 				// Handle the client request
		// 				Request request(buffer);
		// 				request.parseRequest();
		// 				if (pfds[index].revents & POLLOUT)
		// 					sendResponse(client_fd, request);
		// 			}
		// 		}
		// 		++index;
		// 		++it;
		// 	}
		// }
}

int Server::getServerFd() const { return serverFd; }

struct sockaddr *Server::getAddress() const { return  ((struct sockaddr *)&address); }

socklen_t *Server::getAddrlen() const { return ((socklen_t*)&addrlen); }

const ServerTraits& Server::getConf() const { return (conf); }

Server::~Server() {}

int	Server::is_dir(const char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) != 0)
		return (0);
	return (S_ISDIR(statbuf.st_mode));
}