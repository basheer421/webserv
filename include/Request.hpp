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

class Request
{
    private:
        std::map<std::string, std::string>  _request;
        std::string                         _buff;
        std::string                         _reqUrl;
		bool								_isUrlCgi;

    public:
        Request();
        Request(std::string buffer);
        Request(const Request &object);
        Request &operator=(const Request &rhs);
        ~Request();
        void    parseRequest();
        bool    isWhiteSpace(std::string    str1);
};
