/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerTraits.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 03:08:40 by bammar            #+#    #+#             */
/*   Updated: 2023/09/24 15:34:30 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerTraits.hpp"
#include "webserv.hpp"

ServerTraits::ServerTraits()
{
	routes["/"].root = root;
	client_max_body_size = 1024;
	root = "/var/www/html";
	cgi_extensions.push_back("");
}

ServerRoute::ServerRoute()
{
	limit_except.reserve(3);
	limit_except.push_back("GET");
	limit_except.push_back("POST");
	limit_except.push_back("DELETE");

	index.push_back("index");
	index.push_back("index.html");

	autoindex = false;
}
