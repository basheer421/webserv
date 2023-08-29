/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 17:06:57 by bammar            #+#    #+#             */
/*   Updated: 2023/08/29 23:27:00 by bammar           ###   ########.fr       */
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

void ServerManager::ProcessResponse(Request& request, Response& res)
{
	in_port_t req_port;
	in_addr_t req_address;

	this->envMap = request.modifyEnv(this->envMap);
	std::map<std::string, std::string>  reqMap = request.getRequest();
	ft::string url(  request.getReqUrl() );
	string host = request.getHost();

	// Throws
	setAddress(host, req_address, req_port);

	std::vector<Server>::iterator server_it = findServer(servers.begin(), servers.end(), req_address, req_port);
	if (server_it == servers.end())
		throw std::runtime_error("400");
	const ServerTraits& conf = (*server_it).getConf();
	ft::string path = conf.root + url;

	/**
	 * Checking if the url has the request method allowed
	 * If not "405 Method Not Allowed" is throwed.
	 * Searching for the given url inside limit_except for this path, if it's found then throw 405.
	*/
	std::map<ft::string, ServerRoute>::const_iterator route_it;
	route_it = conf.routes.find(url);
	if (route_it != conf.routes.end())
	{
		const std::pair<ft::string, ServerRoute>& foundDir = *route_it;
		if (std::find(foundDir.second.limit_except.begin(), foundDir.second.limit_except.end(), url) != foundDir.second.limit_except.end())
			throw std::runtime_error("405");
	}

	/**
	 * If the conf contains an index
	 */
	if (!conf.index.empty())
	{
		if (is_file(path.c_str()))
		{
			try {
				res.setBody(path);
				res.setResponseHeader("200", "OK");
			} catch (std::exception& e) {

				// This means that the body was not found so we throw 404
				throw (std::runtime_error("404"));
			}
		}
		else
		{
			// Didn't find the dir
			if (route_it == conf.routes.end())
				throw std::runtime_error("404");

			// Found the dir
			const std::pair<ft::string, ServerRoute>& foundDir = *route_it;
			if (foundDir.second.autoindex == true)
			{
				res.setBody(path, true);
				res.setResponseHeader("200", "OK");
				return ;
			} else {
				// try responding the index from index
				// for (size_t i = 0; i < conf.index.size(); ++i)
				// {
				// 	if (is_file(conf.root + "/" + conf.index[i]))
				// 	{
				// 		res.setBody(conf.root + "/" + conf.index[i]);
				// 		break ;
				// 	}
				// }
			}
			
		}
	}
	// Doesn't have index
	else
	{
		if (is_dir(path))
			throw (std::runtime_error("404"));
		if (is_file(path))
		{
			try {
				res.setBody(path);
				res.setResponseHeader("200", "OK");
			} catch (std::exception& e) {

				// This means that the body was not found so we throw 404
				throw (std::runtime_error("404"));
			}
		}
	}
	// if (request.isUrlCgi() == true)
	// 	handleCgi(res, request);
	// else
	// {
		// res.setResponseHeader("200", "OK");
		
		// res.setBody(path);
		std::cout << "sending =============================>  {" << path << "}\n";
	// }	
	
}

Response ServerManager::ManageRequest(const string& buffer)
{
	Request  request(buffer);
	Response response;

	try
	{
		request.parseRequest();
		ProcessResponse(request, response); // server can be known from the request
	}
	catch(std::exception& e)
	{
		int n = ft::from_string<int>(string(e.what()));
		/**
		 * More cases for the error codes can be added later on.
		 */
		switch (n)
		{
			case 400:
				response.setResponseHeader("400", "Bad Request");
				response.setBody("error_pages/400.html");
				break;
			case 403:
				response.setResponseHeader("403", "Forbidden");
				response.setBody("error_pages/403.html");
				break;
			case 404:
				response.setResponseHeader("404", "Not Found");
				response.setBody("error_pages/404.html");
				break;
			case 405:
				response.setResponseHeader("405", "Method Not Allowed");
				response.setBody("error_pages/405.html");
				break;
			default:
				break;
		}
	}
	
	return (response);
}

void ServerManager::run(char **envp)
{
	this->parseEnv(envp);
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
				requestBuilder[client] = "";
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
				char buffer[30000];

                std::memset(buffer, 0, 30000);
				int read_res = recv(pfd.fd, buffer, 29999, 0);
				if (read_res < 0)
				{
					requestBuilder[pfd.fd].clear();
					close(pfd.fd);
					it = sockets.erase(it);
					continue ;
				}
				else
				{
					requestBuilder[pfd.fd] += buffer;

					if (pfd.revents & POLLOUT)
					{
						Response res = ManageRequest(requestBuilder[pfd.fd]);
						send(pfd.fd, res.getResponse().c_str(), res.getResponse().length(), 0);
						requestBuilder[pfd.fd].clear();
					}
				}
			}
		}
	}
}

std::string	ServerManager::strToUpper(std::string str)
{
    for(size_t i = 0; i < str.length(); i++) {
        str[i] = toupper(str[i]);
    }
	return (str);
}

void	ServerManager::parseEnv(char **rawEnv)
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
	}
}

std::map<std::string, std::string> ServerManager::getEnv() const
{
	return envMap;
}