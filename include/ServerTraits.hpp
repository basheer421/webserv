/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerTraits.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 17:39:57 by bammar            #+#    #+#             */
/*   Updated: 2023/09/14 19:20:32 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <map>
#include <ft_string.hpp>
#include <netinet/in.h>

using namespace ft;

/**
 * @brief Struct to store the info of a server route, eg. [location /]
 * 
 * @param limit_except A vector to store the allowed http methods.
 * 	eg. {"GET", "POST", "DELETE"}
 * 
 * @param autoindex A bool to tell if a directory should be listed or not
 * 	when a directory link is requested.
 * 
 * @param try_files A vector that stores the files to be responded after a request,
 * 	the default is just the requested and it returns error if not found.
 * 	eg. {"$uri", "$uri/", "/index.html"}, $uri expands to the requested.
 * 	if not found it will try the directory {autoindex should be true for this otherwise error 403}
 * 	and if directory is not found respond with server_root/index.html
 * 
 * @param return_ A string that stores a redirection path. note: called "return" in conf file.
 */
struct ServerRoute
{
	ServerRoute();

	std::vector<string> limit_except;
	bool autoindex;
	// std::vector<string> try_files;
	string return_;
	std::vector<string> index;
	string root;
};

/**
 * @param root A string that stores the path to the server root.
 * 
 * @param index A vector of file names, relative to path.
 * 
 * @param listen_address A 32 bit int that stores the address to listen to.
 * 
 * @param listen_port An unsigned 16 bit int that stores the port to listen to.
 * 
 * @param server_name A vector to store the server names.
 * 
 * @param client_max_body_size A size_t that stores the amount of bytes
 * 	that can be sent in the response body.
 * 
 * @param routes A vector of ServerRoute struct, that stores the routes info.
 * 
*/
struct ServerTraits
{
	// string root;
	// std::vector<string> index;
	in_addr_t listen_address;
	in_port_t listen_port;
	std::vector<string> server_name;
	size_t client_max_body_size;
	std::map<string, ServerRoute> routes;
};
