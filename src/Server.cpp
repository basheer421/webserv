/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 22:26:15 by bammar            #+#    #+#             */
/*   Updated: 2023/07/31 20:13:55 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::ServerException::ServerException(const ft::string& msg) : msg(msg) {}

const char *Server::ServerException::what() const throw()
{
	return (msg.c_str());
}

void Server::sendResponse(const int& client)
{
	ft::string res_body(
		"<html>"
		"<head><title>404 Not Found</title></head>"
		"<body>"
		"<h1 style='text-align:center; marign:2rem; margin-top:3rem;'>404 Not Found</h1>"
		"<hr>"
		"<h2 style='text-align:center;'>webserv</h2>"
		"</body>"
		"</html>"
	);

	ft::string response (
		"HTTP/1.1 404 Not Found" CRLF
		"Content-Type: text/html; charset=utf-8" CRLF
		"Content-Length: " + ft::to_string(res_body.length()) + CRLF
		CRLF
	);
	response += res_body;
	send(client, response.c_str(), response.length(), 0);
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
	fcntl(server_fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	int optval = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) < 0)
		throw ServerException("setsockopt Error");
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
	std::cout << "Waiting for connections on PORT: " << PORT << "\n";
	while (true)
	{
		int client;
		struct pollfd pfds[FD_COUNT];
		pfds[0] = (struct pollfd) {server_fd, POLLIN, 0};
		
		int index = 0;
		for (std::list<int>::iterator it = clients.begin(); it != clients.end(); ++it)
			pfds[++index] = (struct pollfd) {*it, POLLIN, 0};
		
		if (poll(pfds, clients.size() + 1, -1) < 0)
			throw ServerException("Poll Error");
		
		if (pfds[0].revents & POLLIN)
		{
			client = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
			if (client < 0)
				throw ServerException("AcceptException");
			clients.push_back(client);
		}

		index = 1;
		for (std::list<int>::iterator it = clients.begin(); it != clients.end();) {
			int client_fd = *it;
			if (pfds[index].revents & POLLIN) {
				char buffer[30000] = {0};
				int read_res = recv(client_fd, buffer, 29999, 0);
				if (read_res <= 0) {
					it = clients.erase(it);
					close(client_fd);
					continue;
				} else {
					// Handle the client request
					Request request(buffer);
					request.parseRequest();

					sendResponse(client_fd);

				}
			}
			index++;
			++it;
		}
	}
}

Server::~Server() {}
