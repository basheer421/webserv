/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 17:06:57 by bammar            #+#    #+#             */
/*   Updated: 2023/08/23 18:10:59 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerManager.hpp"

static std::vector<Server>::iterator findServer(std::vector<Server>::iterator start, std::vector<Server>::iterator end, in_addr_t address, in_port_t port)
{
	std::vector<Server>::iterator it;

	for (it = start; it != end; ++it)
	{
		if ((((*it).getConf().listen_address == address) || ((*it).getConf().listen_address == htonl(INADDR_ANY)))
			&& (*it).getConf().listen_port == port)
			return (it);
	}
	return (it);
}

ServerManager::ServerManager(const std::vector<ServerTraits>& cnf)
{
	for (size_t i = 0; i < cnf.size(); ++i)
	{
		servers.push_back( Server(cnf[i]) );

		sockets.push_back((struct pollfd){
			servers.back().getServerFd(),
			POLLIN,
			0
		});
	}
	
}

ServerManager::ServerManager(const ServerManager& src)
{
	if (this == &src)
		return ;
	*this = src;
}

ServerManager& ServerManager::operator = (const ServerManager& src)
{
	if (this == &src)
		return *this;
	this->sockets = src.sockets;
	return *this;
}

ServerManager::~ServerManager() {}

/**
 * Only handles GET requests for now
*/
void ServerManager::sendResponse(const int& client, Request& request)
{
	std::map<std::string, std::string>  reqMap = request.getRequest();
	ft::string url(  request.getReqUrl() );
	
	// will be like "localhost:8080"

	in_port_t req_port;
	in_addr_t req_address;
	string host = request.getHost();

	// Throws
	setAddress(host, req_address, req_port);
	std::cout << "------------host: " << htonl(req_address) << "------" << htons(req_port) << "-----\n";


	std::vector<Server>::iterator server_it = findServer(servers.begin(), servers.end(), req_address, req_port);
	if (server_it == servers.end())
		throw std::runtime_error("Error: Wrong host was found in the request");
	const ServerTraits& conf = (*server_it).getConf();

	std::string response;
	ft::string res_body;

	try {
		response = "HTTP/1.1 200 OK" CRLF;
		ft::string path = conf.root + url;
		if (Server::is_dir(path.c_str()))
		{
			res_body = dirList(path);
		}
		else
			res_body = ft::file_to_string(path);
		std::cout << "sending =============================>  {" << path << "}\n";
	} catch (std::exception& e) {
		response = "HTTP/1.1 404 Not Found" CRLF;
		try {
			res_body = ft::file_to_string("error_pages/404.html");
		} catch (std::exception& e) {
			std::cerr << "404.html not found!\n";
		}
	}
	response +=
		"Content-Type: text/html; charset=utf-8" CRLF
		"Content-Length: " + ft::to_string(res_body.length()) + CRLF
		CRLF
	;
	response += res_body;
	send(client, response.c_str(), response.length(), 0);
}

void ServerManager::run()
{
	while (true)
	{
		// All servers and clients will be inside the poll.
		if (poll(sockets.data(), sockets.size(), -1) < 0)
			throw std::runtime_error("Poll Error");

		/**
		 * loop through servers and check if any is ready to read
		 * 	and then accept the connection.
		*/
		for (size_t i = 0; i < servers.size(); ++i)
		{
			if (sockets[i].revents & POLLIN)
			{
				int client = accept(
					sockets[i].fd,
					servers[i].getAddress(),
					servers[i].getAddrlen()
				);

				if (client < 0)
					throw std::runtime_error("Accept Error");
				sockets.push_back( (struct pollfd) {client, POLLIN | POLLOUT, 0} );
			}
		}

		// loop on the clients
		for (std::vector<struct pollfd>::iterator
			it = sockets.begin() + servers.size();
			it != sockets.end();
			++it)
		{
			struct pollfd& pfd = *it;
			if (pfd.revents & POLLIN)
			{
				char buffer[30000] = {0};
				int read_res = recv(pfd.fd, buffer, 29999, 0);
				if (read_res < 0)
				{
					close(pfd.fd);
					it = sockets.erase(it);
					continue ;
				}
				else
				{
					// Handle the client request
					Request request(buffer);
					request.parseRequest();
					if (pfd.revents & POLLOUT)
						sendResponse(pfd.fd, request); // server can be known from the request
				}
			}
		}
	}
}
