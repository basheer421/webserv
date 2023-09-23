/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 17:07:06 by bammar            #+#    #+#             */
/*   Updated: 2023/09/23 11:47:36 by mkhan            ###   ########.fr       */
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
    std::map<int, bool> isReqCompleteMap;

public:
	ServerManager(const std::vector<ServerTraits>& cnf);
	ServerManager(const ServerManager& src);
	ServerManager& operator = (const ServerManager& src);
	~ServerManager();

	void ProcessResponse(Request& request, Response& res);
	Response ManageRequest(const string&buffer);
	bool redirect(const string& url, const ServerTraits& conf,
		Response& response);

	void run(char **envp); // Throws 
    // void handleCgi(Response &res, Request &req); 

	void	parseEnv(char **rawEnv);
	bool	partialRequest(std::string	buff);
	std::map<std::string, std::string> getEnv() const; 
	std::string	strToUpper(std::string str); 
};
