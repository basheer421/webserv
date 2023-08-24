/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 22:18:03 by bammar            #+#    #+#             */
/*   Updated: 2023/08/24 13:26:59 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

class Server
{

private:
	struct sockaddr_in address;
	std::list<int> clients;
	int addrlen;
	int serverFd;
	ServerTraits conf;

	// There shouldn't be a default constructor
	Server();

public:

	Server(const ServerTraits& cnf);
	Server(const Server& src);
	Server& operator = (const Server& src);
	~Server();

	int getServerFd() const;
	struct sockaddr *getAddress() const;
	socklen_t *getAddrlen() const;
	const ServerTraits& getConf() const;
};
