/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 17:06:57 by bammar            #+#    #+#             */
/*   Updated: 2023/08/16 22:09:31 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerManager.hpp"

ServerManager::ServerManager(const std::vector<ServerTraits>& cnf)
{
	for (size_t i = 0; i < cnf.size(); ++i)
		servers.push_back(Server(cnf[i]));
	
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
	this->servers = src.servers;
	return *this;
}

ServerManager::~ServerManager() {}

void ServerManager::run()
{
	while (true)
	{
		int client;
		std::vector<struct pollfd> sockets;


		for (size_t i = 0; i < servers.size(); ++i)
		{
			sockets.push_back((struct pollfd) {servers[i].getServerFd(), POLLIN, 0});
		}
		







		
		int index = 0;
		for (std::vector<int>::iterator it = sockets.begin(); it != sockets.end(); ++it)
			sockets.push_back( (struct pollfd) {*it, POLLIN | POLLOUT, 0} );
		
		if (poll(pfds, sockets.size() + 1, -1) < 0)
			throw ServerException("Poll Error");

		if (pfds[0].revents & POLLIN)
		{
			client = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
			if (client < 0)
				throw ServerException("AcceptException");
			sockets.push_back(client);
		}

		// We have 1 server so count will be 1 here, otherwise the server count.
		index = 1;
		for (std::list<int>::iterator it = sockets.begin(); it != sockets.end();) {
			int client_fd = *it;
			if (pfds[index].revents & POLLIN) {
				// 30000 is temp,
				//	We should put (client_max_body_size + header size)
				char buffer[30000] = {0};
				int read_res = recv(client_fd, buffer, 29999, 0);
				if (read_res <= 0) {
					it = sockets.erase(it);
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
