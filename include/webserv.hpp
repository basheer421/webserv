/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 20:07:27 by bammar            #+#    #+#             */
/*   Updated: 2023/08/29 19:10:33 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <map>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <exception>
#include <algorithm>
#include <poll.h>
#include <sys/stat.h>
#include "ft_string.hpp"
#include "ServerTraits.hpp"
#include "ParserConf.hpp"
#include "Pages.hpp"
#include "Server.hpp"
#include "Request.hpp"
#include "ServerManager.hpp"
#include "Response.hpp"

# define DEFAULT_PORT 8080
# define CRLF "\r\n"
# define FD_COUNT 255
