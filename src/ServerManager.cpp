/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 17:06:57 by bammar            #+#    #+#             */
/*   Updated: 2023/08/24 13:18:27 by mkhan            ###   ########.fr       */
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
void ServerManager::ProcessResponse(Request& request, Response& res)
{
	this->envMap = request.modifyEnv(this->envMap);
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









	// if (request.isUrlCgi() == true)
	// 	handleCgi(res, request);
	// else
	// {
		res.setResponseHeader("200", "OK");
		ft::string path = conf.root + url;
		res.setBody(path);
		std::cout << "sending =============================>  {" << path << "}\n";
	// }	
	
}


Response ServerManager::ManageRequest(char *buffer)
{
	Request  request(buffer);
	Response response;

	try
	{
		request.parseRequest();
		ProcessResponse(request, response); // server can be known from the request
	}
	catch(const std::exception& e)
	{
		if (e.what())
		{
			response.setResponseHeader("404", "Not Found");		
			response.setBody("error_pages/404.html");
		}
		// else if (e.what() == "403")
		// {
			
		// }
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
			}
		}

		// loop on the clients
		// why NEW
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
					close(pfd.fd);
					it = sockets.erase(it);
                    // delete[] buffer;
					continue ;
				}
				else
				{
            		Response res =  ManageRequest(buffer);
					
					if (pfd.revents & POLLOUT)
                    	send(pfd.fd, res.getResponse().c_str(), res.getResponse().length(), 0);
					// delete[] buffer;
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