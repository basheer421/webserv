/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerTraits.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 03:08:40 by bammar            #+#    #+#             */
/*   Updated: 2023/09/23 22:52:02 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerTraits.hpp"

ServerTraits::ServerTraits()
{
	routes["/"].root = root;
}

ServerRoute::ServerRoute()
{
	limit_except.reserve(3);
	limit_except.push_back("GET");
	limit_except.push_back("POST");
	limit_except.push_back("DELETE");

	autoindex = false;
}
