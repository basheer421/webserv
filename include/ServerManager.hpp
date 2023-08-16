/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 17:07:06 by bammar            #+#    #+#             */
/*   Updated: 2023/08/16 22:03:32 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"

class ServerManager
{
private:
	std::vector<Server> servers;

public:
	ServerManager(const std::vector<ServerTraits>& cnf);
	ServerManager(const ServerManager& src);
	ServerManager& operator = (const ServerManager& src);
	~ServerManager();

	void run(); // Throws
};
