/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 22:26:15 by bammar            #+#    #+#             */
/*   Updated: 2023/08/21 14:04:37 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Pages.hpp"

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
		ft::string path = conf[0].root + url;
		if (is_dir(path.c_str()))
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

Server::Server(const std::vector<ServerTraits>& cnf) : conf(cnf)
{
	int optval;

	optval = 1;

	std::memset(&address, 0, sizeof(address));
	address.sin_addr.s_addr = conf[0].listen_address;
	address.sin_port = conf[0].listen_port;
	address.sin_family = AF_INET;
	address.sin_len = sizeof(address);

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
		throw ServerException("Socket Error");
	if (fcntl(server_fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC) < 0)
		throw ServerException("fcntl Error"); 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
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
	conf = src.conf;
}

Server& Server::operator = (const Server& src)
{
	if (this == &src)
		return *this;
	this->address = src.address;
	this->addrlen = src.addrlen;
	this->server_fd = src.server_fd;
	this->conf = src.conf;
	return *this;
}

void Server::run()
{
	std::cout << "Listening on port: " << htons(conf[0].listen_port) << "\n";
	while (true)
	{
		int client;
		struct pollfd pfds[FD_COUNT];
		pfds[0] = (struct pollfd) {server_fd, POLLIN, 0};
		
		int index = 0;
		for (std::list<int>::iterator it = clients.begin(); it != clients.end(); ++it)
			pfds[++index] = (struct pollfd) {*it, POLLIN | POLLOUT, 0};
		
		if (poll(pfds, clients.size() + 1, -1) < 0)
			throw ServerException("Poll Error");

		if (pfds[0].revents & POLLIN)
		{
			client = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
			if (client < 0)
				throw ServerException("AcceptException");
			clients.push_back(client);
		}

		// We have 1 server so count will be 1 here, otherwise the server count.
		index = 1;
		for (std::list<int>::iterator it = clients.begin(); it != clients.end();) {
			int client_fd = *it;
			if (pfds[index].revents & POLLIN) {
				// 30000 is temp,
				//	We should put (client_max_body_size + header size)
				char buffer[30000] = {0};
				int read_res = recv(client_fd, buffer, 29999, 0);
				if (read_res <= 0) {
					it = clients.erase(it);
					close(client_fd);
					continue;
				} 
				else 
				{
					// Handle the client request
					Request request(buffer);
					request.parseRequest();
					if (pfds[index].revents & POLLOUT)
					{
						std::cout << "Sending response to ============================> client_fd: " << client_fd << std::endl;
						sendResponse(client_fd, request);
					}
				}
			}
			++index;
			++it;
		}
	}
}

int	Server::is_dir(const char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) != 0)
		return (0);
	return (S_ISDIR(statbuf.st_mode));
}

Server::~Server() {}

std::string	Server::strToUpper(std::string str)
{
    for(size_t i = 0; i < str.length(); i++) {
        str[i] = toupper(str[i]);
    }
	return (str);
}

void	Server::parseEnv(char **rawEnv)
{
	std::string str1;
	std::string envStr;
	bool	flag = false;

	for (size_t i = 0; rawEnv[i]; i++)
	{
		envStr += rawEnv[i];
		envStr += "\n";
	}
	std::stringstream str(envStr);
	while (getline(str, str1, '\n'))
	{
		std::stringstream line(str1);
		std::string key, value;
		getline(line, key, '=');
		getline(line, value);
		std::map<std::string, std::string>::iterator it;
		for (it = envMap.begin(); it != envMap.end(); ++it)
		{
			key = strToUpper(key);
			if (key == it->first)
				flag = true;
		}
		if (flag)
			continue;
		envMap[key] = value;
		// std::cout << key << " " << envMap[key] << std::endl;
	}
}

std::map<std::string, std::string> Server::getEnv() const
{
	return envMap;
}