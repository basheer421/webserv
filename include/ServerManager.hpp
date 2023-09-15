/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 17:07:06 by bammar            #+#    #+#             */
/*   Updated: 2023/09/14 22:04:39 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp" 

class Cgi;
class Server;
class Request;
class Response;
class ServerManager
{
private:
	std::vector<Server> servers;
	std::vector<struct pollfd> sockets;
	std::map<std::string, std::string> envMap;
	std::map<int, string> requestBuilder;

public:
	ServerManager(const std::vector<ServerTraits>& cnf);
	ServerManager(const ServerManager& src);
	ServerManager& operator = (const ServerManager& src);
	~ServerManager();

	void ProcessResponse(Request& request, Response& res);
	Response ManageRequest(const string&buffer);
	bool redirect(const ServerRoute& route, Response& res);

	void run(char **envp); // Throws 
    // void handleCgi(Response &res, Request &req); 

	void	parseEnv(char **rawEnv);
	bool	partialRequest(std::string	buff);
	std::map<std::string, std::string> getEnv() const; 
	std::string	strToUpper(std::string str); 
};
