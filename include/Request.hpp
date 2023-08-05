/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 18:01:47 by mkhan             #+#    #+#             */
/*   Updated: 2023/07/29 18:01:47 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <map>
#include <sstream>

enum	e_request_type
{
	GET,
	POST,
	DELETE
};

class Request
{
    private:
        std::map<std::string, std::string>  _request;
		e_request_type						_type;
        std::string                         _buff;
        std::string                         _reqUrl;
		std::string							_host;
		bool								_isUrlCgi;

    public:
        Request();
        Request(std::string buffer);
        Request(const Request &object);
        Request &operator=(const Request &rhs);
        ~Request();
        void    parseRequest();
        bool    isWhiteSpace(std::string    str1);

		std::map<std::string, std::string> getRequest() const;
		std::string getReqUrl() const;
		std::string	getHost() const;
		e_request_type getReqType() const;

		bool isUrlCgi() const;
};
