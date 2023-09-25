/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 17:06:57 by bammar            #+#    #+#             */
/*   Updated: 2023/09/25 21:18:35 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerManager.hpp"

bool isExit = false;

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


static std::vector<Server>::iterator findServer(std::vector<Server>::iterator start, std::vector<Server>::iterator end, const string& host)
{
	std::vector<Server>::iterator it;
	string hst = host;
	in_port_t port = 0;
	in_addr_t address = 0;

	if (hst.find(':') == std::string::npos)
		throw std::runtime_error("400");
	string addStr = hst.substr(0, hst.find(':'));
	string portStr = hst.substr(hst.find(':') + 1);
	if (std::count(host.begin(), host.end(), '.') == 3)
	{
		setAddress(hst, address, port);	
	} else {
		setAddress(portStr, address, port);
	}
	
	for (it = start; it != end; ++it)
	{
		if ((((*it).getConf().listen_address == address)
			|| ((*it).getConf().listen_address == htonl(INADDR_ANY)))
			&& (*it).getConf().listen_port == port)
			return (it);
		if (((std::find((*it).getConf().server_name.begin(),
			(*it).getConf().server_name.end(), addStr) != (*it).getConf().server_name.end())
			|| (std::find((*it).getConf().server_name.begin(),
			(*it).getConf().server_name.end(), "_") != (*it).getConf().server_name.end()))
			&& (*it).getConf().listen_port == port)
			return (it);
	}
	return (it);
}

void ServerManager::throwIfnotAllowed(const string& url, const ServerTraits& conf,
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
		case HEAD:
			reqType = "HEAD";
			break;
		case PUT:
			reqType = "PUT";
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
		throw ErrorPage(conf, "405");
}

/**
 * This should be changed in the future to handle multiple redirects.
 * like 301 and 302.
*/
bool ServerManager::redirect(const ServerRoute& route, Response& res)
{
	if (!route.return_.empty())
	{
		res.setResponseHeader("301", "Moved Permanently");

		// Last header
		res.appendHeader("Location: " + route.return_ + CRLF);
		return true;
	}
	return false;
}

/**
 * @brief Get the Dir (if there's a file name it will remove it)
 * 
 * "/home/index.html" -> "/home"
 * "/index.html" -> ""
 * 
 * @param path 
 * @return string 
 */
static string getDir(string path)
{
	size_t pos = path.rfind('/');
	if (pos == std::string::npos)
		throw std::runtime_error("400");
	else
		path = path.substr(0, pos);
	return (path);
}

ServerRoute ServerManager::getRoute(string& url, const ServerTraits& conf)
{
	std::map<ft::string, ServerRoute>::const_iterator route_it;
	route_it = conf.routes.find(url);
	while (route_it == conf.routes.end() && !url.empty())
	{
		url = getDir(url);
		route_it = conf.routes.find(url);
	}
	if (route_it == conf.routes.end() && url.empty())
	{
		url = "/";
		route_it = conf.routes.find(url);
		if (route_it == conf.routes.end())
			throw ErrorPage(conf, "500");
	}
	return route_it->second;
}

void ServerManager::ProcessResponse(Request& request, Response& res)
{
	this->envMap = request.modifyEnv(this->envMap);
	const ft::string& urlx = request.getReqUrl();
	string url = urlx;

	string host = request.getHost();

	if (host.empty())
		throw std::runtime_error("400");
	
	std::vector<Server>::iterator serv_it = findServer(
		servers.begin(), servers.end(), host);
	if (serv_it == servers.end())
		throw std::runtime_error("400");

	// Getting the server
	const ServerTraits& conf = (*serv_it).getConf();

	if (url.back() == '/' && url.size() > 1)
		url.resize(url.size() - 1);
	string routeUrl = url;
	ServerRoute route = getRoute(routeUrl, conf);
	if (route.root.empty())
		route.root = conf.root;
    if (conf.client_max_body_size < (request.getContLen() + request.getHeaderLength())) {
        throw ErrorPage(conf, "400");
	}

	// Changing the path to be the full path
	string path = route.root + "/" + url.substr(routeUrl.length());
	if (path.back() == '/')
		path.resize(path.size() - 1);

	// Checking if the url has the request method allowed
	throwIfnotAllowed(url, conf, request);


	// Checking if there's a redirect
	if (redirect(route, res))
		return ;

	if ((request.getReqType() == DELETE || request.getReqType() == POST || request.getReqType() == PUT) && !request.isUrlCgi())
	{
		res.setBody(path, request);
		return ;
	}

	if (is_file(path))
	{
		if (request.isUrlCgi())
		{
			Cgi cgi;

			cgi.SetEnv(this->envMap, res, request);
			cgi.HandleCgi(res, request, conf.root, conf);
		}
		else
			res.setBody(path, request);
		return ;
	}

	if (!is_dir(path))
		throw (ErrorPage(conf, "404"));

	std::map<ft::string, ServerRoute>::const_iterator route_it(
		conf.routes.find(url)
	);

	// Didn't find the dir
	if (route_it == conf.routes.end())
		throw ErrorPage(conf, "404");

	/**
	 * By now we know that the url is a directory.
	 */

	if (!route.index.empty())
	{
		// Responding with index
		for (size_t i = 0; i < route.index.size(); ++i)
		{
			if (is_file(route.root + "/" + route.index[i]))
			{
				res.setBody(route.root + "/" + route.index[i], request.getReqUrl());
				return ;
			}
		}

		// Responding with autoindex if found
		if (route.autoindex == true)
		{
			if (!is_dir(path))
				throw (ErrorPage(conf, "404"));
			res.setBody(path, request, true);
			return ;
		}
		throw (ErrorPage(conf, "404"));
	}

	/**
	 * If the conf doesn't contain an index
	*/
	else
	{
		// Responding with autoindex if found
		if (!is_dir(path) || route.autoindex == false)
			throw (ErrorPage(conf, "404"));
		res.setBody(path, request, true);
		return ;
	}

	// Should be already found, otherwise 500 is thrown.
	throw (ErrorPage(conf, "500"));
}

static void setErrPage(Response& response, const Request& request, const string& code,
	const string& mssg, const ServerTraits& conf)
{
	response.setResponseHeader(code, mssg);
	string page;
	if (conf.error_pages.find(code) != conf.error_pages.end())
		page = conf.error_pages.find(code)->second;
	else
		page = getErrPage(code, mssg);
	
	response.setErrBody(page, request);
}

static void setDefaultErrPage(Response& response, const Request& request, const string& code,
	const string& mssg)
{
	response.setResponseHeader(code, mssg);
	string page = getErrPage(code, mssg);
	response.setErrBody(page, request);
}

Response ServerManager::ManageRequest(const string& buffer)
{
	Request  request(buffer);
	Response response;

	string arr[] = {"400", "403", "404", "405", "500", "504"};
	string msgArr[] = {"Bad Request", "Forbidden", "Not Found", "Method Not Allowed",
		"Internal Server Error", "Gateway Timeout"};
	try
	{
		request.parseRequest(true);
		ProcessResponse(request, response);
	}
	catch (const ErrorPage& e)
	{
		string what = e.what();

		setErrPage(response, request, what, msgArr[(std::find(arr, arr + 6, what) - arr)], e.getConf());
	}
	catch (const std::runtime_error& e)
	{
		string what = e.what();

		setDefaultErrPage(response, request, what, msgArr[(std::find(arr, arr + 6, what) - arr)]);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		response.setResponseHeader("500", "Internal Server Error");
		response.setErrBody(getErrPage("500", "Internal Server Error"), request);
	}
	return (response);
}

static void handle_exit(int sig)
{
    (void)sig;
    isExit = true;
}

void ServerManager::run(char **envp)
{
	this->parseEnv(envp);
	signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, handle_exit);

	while (!isExit)
	{
        
		// All servers and clients will be inside the poll.
		if (poll(sockets.data(), sockets.size(), -1) < 0){
			throw std::runtime_error("Poll Error");
        }

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
