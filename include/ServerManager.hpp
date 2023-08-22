/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 17:07:06 by bammar            #+#    #+#             */
/*   Updated: 2023/08/20 16:42:58 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

class Server;
class Request;

class ServerManager
{
private:
	std::vector<Server> servers;
	std::vector<struct pollfd> sockets;


public:
	ServerManager(const std::vector<ServerTraits>& cnf);
	ServerManager(const ServerManager& src);
	ServerManager& operator = (const ServerManager& src);
	~ServerManager();

	void sendResponse(const int& client, Request& request);

	void run(); // Throws
};
