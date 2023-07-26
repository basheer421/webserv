/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 20:07:27 by bammar            #+#    #+#             */
/*   Updated: 2023/07/26 22:53:19 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <cstring>
#include <exception>
#include <poll.h>
#include <list>
#include "ft_string.hpp"
#include "Server.hpp"
#include "ParserConf.hpp"

# define PORT 8080
# define CRLF "\r\n"
# define FD_COUNT 255
