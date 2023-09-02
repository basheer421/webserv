/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 20:07:27 by bammar            #+#    #+#             */
/*   Updated: 2023/09/03 00:10:02 by bammar           ###   ########.fr       */
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
#include <sys/wait.h>
#include <sys/types.h>
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
#include "Cgi.hpp"

# define DEFAULT_PORT 8080
# define CRLF "\r\n"
# define FD_COUNT 255
