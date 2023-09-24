/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerTraits.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 03:08:40 by bammar            #+#    #+#             */
/*   Updated: 2023/09/24 18:12:09 by mkhan            ###   ########.fr       */
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
	limit_except.reserve(5);
	limit_except.push_back("GET");
	limit_except.push_back("HEAD");
	limit_except.push_back("PUT");
	limit_except.push_back("POST");
	limit_except.push_back("DELETE");

	index.push_back("index");
	index.push_back("index.html");

	autoindex = false;
}
