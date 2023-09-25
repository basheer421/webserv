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
	HEAD,
	PUT,
	DEFAULT
};

class Request
{
	private:
		std::map<std::string, std::string>	_request;
		std::map<std::string, std::string>	_queryMap;
		e_request_type						_type;
		std::string							_buff;
		std::string							_buffCopy;
		std::string							_reqUrl;
		std::string							_host;
		std::string							_postBody;
		size_t								_contLen;
		bool								_isUrlCgi;
		bool								_postFlag;
		bool								_isFileUpload;
		std::string							_putCode;
		std::string							_queryString;
		std::string							queryURl;
		std::string							deleteURl;
		bool								_flagEnc;

	public:
		Request();
		Request(std::string buffer);
		Request(const Request &object);
		Request &operator=(const Request &rhs);
		~Request();

		std::map<std::string, std::string>	modifyEnv(std::map<std::string, std::string> env);
		std::map<std::string, std::string>	getRequest() const;
		std::map<std::string, std::string>	parseUnderScore();

		e_request_type						getReqType() const;
		std::string							getReqUrl() const;
		std::string							getHost() const;
		std::string							getPostBody() const;
		std::string							getCgiUrl() const;
		std::string							strToUpper(std::string str);
		std::string							getPutCode() const;
		std::string							replaceChar(std::string str);
		std::string							getQueryString() const;
		std::string							getStrRequestType() const;
		std::string							getQueryUrl() const;
		std::string							getDeleteURL() const;

		void								setDeleteURL();
		size_t								getContLen();
		int									getHeaderLength();
		int									hexadecimalToDecimal(string hexVal);
		bool								isWhiteSpace(std::string	str1);
		bool								isUrlCgi() const;
		void								parseRequest(bool flag=false);
		void								parsePostBody();
		void								fileUpload();
		void								parseChunkedBody();
		void								parseHexReqUrl();
		void								parseQueryUrl();
		void								headerValidation();
		void								setReqType(const std::string &key);
        std::string                         replaceotherChar(std::string str);
};
