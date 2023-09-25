/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 17:07:06 by bammar            #+#    #+#             */
/*   Updated: 2023/09/25 18:51:47 by bammar           ###   ########.fr       */
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

	class ErrorPage : public std::exception
	{
	private:
		const ServerTraits conf;
		const std::string msg;

	public:
		ErrorPage(const ServerTraits& conf, const std::string& msg) : conf(conf), msg(msg) {}

		virtual const char* what() const throw()
		{
			return msg.c_str();
		}

		const ServerTraits& getConf() const
		{
			return conf;
		}
		virtual ~ErrorPage() throw() {}
	};


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
	static void throwIfnotAllowed(const string& url, const ServerTraits& conf,
		const Request& request);
	static ServerRoute getRoute(string& url, const ServerTraits& conf);
};
