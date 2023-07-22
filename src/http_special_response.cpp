/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_special_response.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 01:39:30 by bammar            #+#    #+#             */
/*   Updated: 2023/07/23 01:50:42 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

namespace webserv
{
	// Will add more later.
	static ft::string page_404 (
		"<html>" CRLF
		"<head><title>404 NOT FOUND</title></head>" CRLF
		"<body>" CRLF
		"<h1 style='text-align=center; color:red;'>404 NOT FOUND</h1>" CRLF
	);
}
