/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 20:07:27 by bammar            #+#    #+#             */
/*   Updated: 2023/09/23 13:41:38 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <stdio.h>
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
#include <sys/wait.h>
#include <signal.h>
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
# define BUFFER_SIZE 216000

