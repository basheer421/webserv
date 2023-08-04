/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 22:26:15 by bammar            #+#    #+#             */
/*   Updated: 2023/08/04 16:38:06 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::ServerException::ServerException(const ft::string& msg) : msg(msg) {}

const char *Server::ServerException::what() const throw()
{
	return (msg.c_str());
}

/**
 * Only handles GET requests for now
*/
void Server::sendResponse(const int& client, Request& request)
{
	// request is not used now cuz parsing is not done
	(void) request;

	std::map<std::string, std::string>  reqMap = request.getRequest();
	ft::string url(  request.getReqUrl());

	std::string response;
	ft::string res_body;

	try {
		response = "HTTP/1.1 200 OK" CRLF;
		ft::string path = (conf.at("http-server-location /").directives.at("root").at(0)) + url;
		std::cout << "------------sending-->  {" << path << "}\n";
		res_body = ft::file_to_string(path);
		std::cout << "body >> \n";
		std::cout << res_body << "\n";
		std::cout << "body << \n";
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

/**
 * TODO:
 * https://nginx.org/en/docs/http/ngx_http_core_module.html#listen
 * listen address[:port]
 * listen port
 * listen unix:path
 * Default:	listen *:80 | *:8000;
 * 
 * https://stackoverflow.com/questions/15673846
*/
void Server::addressInit()
{
	// Default
	ft::string conf_addrs = "*:" + ft::to_string(DEFAULT_PORT);

	std::map<ft::string, ParserConf::Module>::const_iterator conf_server;
	conf_server = conf.find("http-server");
	if (conf_server != conf.end())
	{
		std::map<ft::string, ParserConf::Directive>::const_iterator listen_it;
		listen_it = (*(conf_server)).second.directives.find("listen");
		if (listen_it != (*conf_server).second.directives.end())
		{
			if ((*listen_it).second.size() != 0)
				conf_addrs = (*listen_it).second.at(0);
		}
	}
	setAddress(conf_addrs);
}

void Server::setAddress(ft::string& addrs)
{
    std::vector<ft::string> vec = addrs.split(':');

    if (vec.size() != 1 && vec.size() != 2)
        throw ServerException("Bad Address: Invalid format");

    addrlen = sizeof(address);
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    // Only port provided
    if (vec.size() == 1)
    {
        in_port_t port = ft::from_string<in_port_t>(vec.at(0));
        if (port <= 0)
            throw ServerException("Bad Address: Invalid port number");
        address.sin_port = htons(port);
        return;
    }

    // Handling address
    if (vec.at(0) != "*")
    {
        std::vector<ft::string> segments = vec.at(0).split('.');
        if (segments.size() != 4)
            throw ServerException("Bad Address: Invalid IP address format");

        in_addr_t adrs_int = (ft::from_string<in_addr_t>(segments.at(0)) << 24)
                           | (ft::from_string<in_addr_t>(segments.at(1)) << 16)
                           | (ft::from_string<in_addr_t>(segments.at(2)) << 8)
                           | ft::from_string<in_addr_t>(segments.at(3));
        address.sin_addr.s_addr = htonl(adrs_int);
    }

    // Handling port
    in_port_t port = ft::from_string<in_port_t>(vec.at(1));
	std::cout << "string: " << vec.at(1) << "\n";
	std::cout << "num: " << port << "\n";
    address.sin_port = htons(port);
}


Server::Server()
{
	*this = Server(std::map<ft::string, ParserConf::Module>());
}

Server::Server(const std::map<ft::string, ParserConf::Module>& cnf) : conf(cnf)
{
	addressInit();
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
		throw ServerException("Socket Error");
	if (fcntl(server_fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC) < 0)
		throw ServerException("fcntl Error"); 
	int optval = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) < 0)
		throw ServerException("setsockopt Error");
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		throw ServerException("Bind Error");
	}
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
	std::cout << "Waiting for connections on PORT: " << htons(address.sin_port) << "\n";
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

					sendResponse(client_fd, request);

				}
			}
			index++;
			++it;
		}
	}
}

Server::~Server() {}
