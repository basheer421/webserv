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

#include "webserv.hpp"

enum	e_request_type
{
	GET,
	POST,
	DELETE,
	DEFAULT
};

class Request
{
    private:
        std::map<std::string, std::string>  _request;
		std::map<std::string, std::string>	_queryMap;
		e_request_type						_type;
        std::string                         _buff;
        std::string                         _buffCopy;
        std::string                         _reqUrl;
		std::string							_host;
		bool								_isUrlCgi;
		bool								_postFlag;
		std::string							_postBody;

    public:
        Request();
        Request(std::string buffer);
        Request(const Request &object);
        Request &operator=(const Request &rhs);
        ~Request();

        void    parseRequest();
        bool    isWhiteSpace(std::string    str1);
		void	parsePostBody();
		void	parseChunkedBody();
		void	parseHexReqUrl();
		void	parseQueryUrl();
		void	headerValidation();
		std::map<std::string, std::string> modifyEnv(std::map<std::string, std::string> env);

		std::map<std::string, std::string> getRequest() const;
		std::string getReqUrl() const;
		std::string	getHost() const;
		std::string getPostBody() const;
		e_request_type getReqType() const;
        std::string getCgiUrl() const;
		std::string	strToUpper(std::string str);
		std::map<std::string, std::string>	parseUnderScore();
		std::string replaceChar(std::string str);

		int hexadecimalToDecimal(string hexVal);

		bool isUrlCgi() const;
};
