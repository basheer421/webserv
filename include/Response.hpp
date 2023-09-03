/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 14:05:44 by mkhan             #+#    #+#             */
/*   Updated: 2023/09/02 13:29:24 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

#include "webserv.hpp"
#include <string>

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

	public:
		Response();
		Response(Response const &object);
		Response &operator=(Response const &rhs);
		~Response();

		std::string	getHeader();
		void		setHeader();
		void		setBody(const std::string& path, bool autoindex=false);
		void		setCode(std::string str);
		void		setMssg(std::string str);
		void		setCtype(std::string str);
        void        setCgiBody(std::string str);
		void		setClen(int len);
		void		setResponseHeader(std::string code, std::string mssg);
		std::string	getResponse();
		void 		appendHeader(const std::string& str);
};

int	is_dir(const string path);
int is_file(const string path);
