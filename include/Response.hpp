/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 14:05:44 by mkhan             #+#    #+#             */
/*   Updated: 2023/08/31 18:06:37 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

#include "webserv.hpp"
#include <string>
#include <map>

class Response
{
	private:
		std::string header;
		std::string code;
		std::string	mssg;
		std::string	content_type;
		int			content_len;
		std::string	res_body;
		std::string res;
		std::map<std::string, std::string>	mimes;

	public:
		Response();
		Response(Response const &object);
		Response &operator=(Response const &rhs);
		~Response();

		std::string	getHeader();
		void		setHeader();
		void		setBody(std::string	path);
		void		setCode(std::string str);
		void		setMssg(std::string str);
		void		setCtype(std::string str);
        void        setCgiBody(std::string str);
		void		setClen(int len);
		void		setResponseHeader(std::string code, std::string mssg);
		int			is_dir(const char *path);
		void		parseMimes();
		std::string	getResponse();
};
