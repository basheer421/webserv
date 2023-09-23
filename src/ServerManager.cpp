/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 17:06:57 by bammar            #+#    #+#             */
/*   Updated: 2023/09/23 14:05:29 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerManager.hpp"

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


static std::vector<Server>::iterator findServer(std::vector<Server>::iterator start, std::vector<Server>::iterator end, in_addr_t address, in_port_t port)
{
	std::vector<Server>::iterator it;

	for (it = start; it != end; ++it)
	{
		if ((((*it).getConf().listen_address == address)
			|| ((*it).getConf().listen_address == htonl(INADDR_ANY)))
			&& (*it).getConf().listen_port == port)
			return (it);
	}
	return (it);
}

static void throwIfnotAllowed(const string& url, const ServerTraits& conf,
	const Request& request)
{
	std::map<ft::string, ServerRoute>::const_iterator route_it;
	route_it = conf.routes.find(url);
	if (route_it == conf.routes.end())
		return ;
	const std::pair<ft::string, ServerRoute>& foundDir = *route_it;
	string reqType;
	switch (request.getReqType())
	{
		case GET:
			reqType = "GET";
			break;
		case POST:
			reqType = "POST";
			break;
		case DELETE:
			reqType = "DELETE";
			break;
		default:
			reqType = "GET";
			break;
	}

	// Checking if the request type is allowed
	if (std::count(foundDir.second.limit_except.begin(),
			foundDir.second.limit_except.end(), reqType) == 0)
		throw std::runtime_error("405");
}

/**
 * This should be changed in the future to handle multiple redirects.
 * like 301 and 302.
*/
bool ServerManager::redirect(const string& url, const ServerTraits& conf,
	Response& res)
{
	std::map<ft::string, ServerRoute>::const_iterator route_it;
	route_it = conf.routes.find(url);
	if (route_it == conf.routes.end())
		return false;
	const std::pair<ft::string, ServerRoute>& foundDir = *route_it;

	if (!foundDir.second.return_.empty())
	{
		res.setResponseHeader("301", "Moved Permanently");

		// Last header
		res.appendHeader("Location: " + foundDir.second.return_ + CRLF);
		return true;
	}
	return false;
}

void ServerManager::ProcessResponse(Request& request, Response& res)
{
	this->envMap = request.modifyEnv(this->envMap);
	const ft::string& url = request.getReqUrl();

	in_port_t req_port;
	in_addr_t req_address;
	string host = request.getHost();

	if (host.empty())
		throw std::runtime_error("400");

	// Throws 500
	setAddress(host, req_address, req_port);

	
	std::vector<Server>::iterator serv_it = findServer(
		servers.begin(), servers.end(), req_address, req_port);
	if (serv_it == servers.end())
		throw std::runtime_error("400");

	// Getting the server
	const ServerTraits& conf = (*serv_it).getConf();

	ft::string path = (conf.root + url);

	// Getting back the spaces
	path = path.replace_all("%20", " ");

	// Checking if the url has the request method allowed
	throwIfnotAllowed(url, conf, request);

	// Checking if there's a redirect
	if (redirect(url, conf, res))
		return ;

	if (request.getReqType() == DELETE)
	{
		res.setBody(path, request);
		return ;
	}
	if (request.getReqType() == POST || request.getReqType() == PUT)
	{
		if (request.getPutCode() == "201")
			res.setResponseHeader("201", "Created");
		// return ;
	}

	if (is_file(path))
	{
		if (request.isUrlCgi())
		{
			Cgi cgi;

			cgi.SetEnv(this->envMap, res, request);
			cgi.HandleCgi(res, request);
		}
        else
		    res.setBody(path, request);
		return ;
	}

	std::map<ft::string, ServerRoute>::const_iterator route_it(
		conf.routes.find(url)
	);
	// Didn't find the dir
	if (route_it == conf.routes.end())
		throw std::runtime_error("404");
	std::cout << url << std::endl;

	const std::pair<ft::string, ServerRoute>& foundDir = *route_it;
	/**
	 * By now we know that the url is a directory.
	 * 
	 * If the conf contains an index
	 * Note: This should be changed to handle multiple indexes
	 * 	and to be by location block not server block.
	 */
	if (!conf.index.empty())
	{
		// Responding with index
		for (size_t i = 0; i < conf.index.size(); ++i)
		{
			if (is_file(conf.root + "/" + conf.index[i]))
			{
				res.setBody(conf.root + "/" + conf.index[i], request);
				return ;
			}
		}

		// Responding with autoindex if found
		if (foundDir.second.autoindex == true)
		{
			if (!is_dir(path))
				throw (std::runtime_error("404"));
			res.setBody(path, request, true);
			return ;
		}
		throw (std::runtime_error("404"));
	}

	/**
	 * If the conf doesn't contain an index
	*/
	else
	{
		// Responding with autoindex if found
		if (!is_dir(path) || foundDir.second.autoindex == false)
			throw (std::runtime_error("404"));
		res.setBody(path, request, true);
		return ;
	}
	// Should be already found, otherwise 500 is thrown.
	throw (std::runtime_error("500"));
}

Response ServerManager::ManageRequest(const string& buffer)
{
	Request  request(buffer);
	Response response;

	try
	{
		request.parseRequest(true);
		ProcessResponse(request, response); // server can be known from the request
	}
	catch(const std::runtime_error& e)
	{
		string what = e.what();
		if (what == "405")
		{
			response.setResponseHeader("405", "Method Not Allowed");
			response.setErrBody(getErrPage("405", "Method Not Allowed"), request);
		}
		else if (what == "404")
		{
			response.setResponseHeader("404", "Not Found");
			response.setErrBody(getErrPage("404", "Not Found"), request);
		}
		else if (what == "403")
		{
			response.setResponseHeader("403", "Forbidden");
			response.setErrBody(getErrPage("403", "Forbidden"), request);
		}
		else if (what == "400")
		{
			response.setResponseHeader("400", "Bad Request");
			response.setErrBody(getErrPage("400", "Bad Request"), request);
		}
		else
		{
			std::cerr << "Error: " << e.what() << std::endl;
			response.setResponseHeader("500", "Internal Server Error");
			response.setErrBody(getErrPage("500", "Internal Server Error"), request);
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		response.setResponseHeader("500", "Internal Server Error");
		response.setErrBody(getErrPage("500", "Internal Server Error"), request);
	}
	return (response);
}


void ServerManager::run(char **envp)
{
	this->parseEnv(envp);
	signal(SIGPIPE, SIG_IGN);

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
                isReqCompleteMap[client] = false;
			}
		}

		// loop on the clients
		for (std::vector<struct pollfd>::iterator
			it = sockets.begin() + servers.size();
			it < sockets.end();
			++it)
		{
			struct pollfd& pfd = *it;

			if (pfd.revents & POLLIN && isReqCompleteMap[pfd.fd] == false)
			{
				char buffer[BUFFER_SIZE];

                std::memset(buffer, 0, BUFFER_SIZE);
				int read_res = recv(pfd.fd, buffer, BUFFER_SIZE - 1, 0);
				if (read_res < 0)
				{
					requestBuilder[pfd.fd].clear();
					close(pfd.fd);
					it = sockets.erase(it);
				}
				else
					requestBuilder[pfd.fd] += buffer;
				isReqCompleteMap[pfd.fd] = partialRequest(requestBuilder[pfd.fd]);
			}
			else if (pfd.revents & POLLOUT && isReqCompleteMap[pfd.fd] == true)
			{
				Response res = ManageRequest(requestBuilder[pfd.fd]);
				// std::cout << "===========================================================================" << std::endl;
				// std::cout << res.getResponse() << std::endl;
				// std::cout << "===========================================================================" << std::endl;
				send(pfd.fd, res.getResponse().c_str(),
					res.getResponse().length(), 0);
				requestBuilder[pfd.fd].clear();
				isReqCompleteMap[pfd.fd] = false;
			}
		}
	}
}

bool	ServerManager::partialRequest(std::string buff)
{
	std::string	first("");
	bool		flag;
	first = buff;
	Request	req(first);
	req.parseRequest();
	std::map<std::string, std::string> reqMap = req.getRequest();
	if (buff.find("\r\n\r\n") == std::string::npos)
		return (false);
	if (buff.empty())
		return (false);
	if (reqMap["Transfer-Encoding:"] == "chunked")
	{
		if (buff.find("0" CRLF CRLF) == std::string::npos)
			return (false);
		flag = true;
	}
	if ((buff.length() < (req.getContLen() + req.getHeaderLength())) && req.getReqType() == POST && !flag)
		return (false);
	return (true);
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
