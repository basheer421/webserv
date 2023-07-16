/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserConf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 12:05:54 by bammar            #+#    #+#             */
/*   Updated: 2023/07/16 18:08:18 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParserConf.hpp"
#include <iostream>

ParserConf::ParserConf() {}

ParserConf::ParserConf(std::string text) : text(text)
{
	iter = this->text.begin();
}

ParserConf::ParserConf(const ParserConf& src)
{
	if (this == &src)
		return ;
	*this = src;
}

ParserConf& ParserConf::operator = (const ParserConf& src)
{
	if (this == &src)
		return *this;
	text = src.text;
	iter = src.iter;
	return *this;
}

ParserConf::~ParserConf() {}

std::string ParserConf::parseWord()
{
	

	while (iter != text.end() && std::isspace(*iter))
		++iter;
	
	std::string::iterator start = iter;
	
	while (iter != text.end() && !std::isspace(*iter))
		++iter;
	return std::string(start, iter); // [start, end)
}

ParserConf::Directive ParserConf::parseDirective()
{
	ParserConf::Directive directive;
	std::vector<std::string> data;

	directive.first = parseWord();
	while (*iter != ';')
		data.push_back(parseWord());
	directive.second = data;
	++iter; // consume ';'
	return (directive);
}

ParserConf::Module ParserConf::parseModule()
{
	Module modls;
	std::string name = parseWord();
	std::string firstWord = parseWord();
	

	if (firstWord == "{")
	{
		modls.name = name;
		std::string word = parseWord();
		while (word != "}")
		{
			iter -= word.length();
			modls.modules.push_back(parseModule());
			word = parseWord();
		}
	}
	else
	{
		iter -= firstWord.length() + name.length();
		modls.name = "";
		modls.directives.push_back(parseDirective());
	}
	return modls;
}

std::vector<ParserConf::Module> ParserConf::parseFile()
{
	std::cout << "file: " << text << "\n";
	std::vector<ParserConf::Module> file;
	while (iter != text.end())
	{
		file.push_back(parseModule());
	}
	return (file);
}

/*
user       www www;  ## Default: nobody
worker_processes  5;  ## Default: 1
error_log  logs/error.log;
pid        logs/nginx.pid;
worker_rlimit_nofile 8192;

events {
  worker_connections  4096;  ## Default: 1024
}

http {
  include    conf/mime.types;
  include    /etc/nginx/proxy.conf;
  include    /etc/nginx/fastcgi.conf;
  index    index.html index.htm index.php;

  default_type application/octet-stream;
  log_format   main '$remote_addr - $remote_user [$time_local]  $status '
    '"$request" $body_bytes_sent "$http_referer" '
    '"$http_user_agent" "$http_x_forwarded_for"';
  access_log   logs/access.log  main;
  sendfile     on;
  tcp_nopush   on;
  server_names_hash_bucket_size 128; # this seems to be required for some vhosts

  server { # php/fastcgi
    listen       80;
    server_name  domain1.com www.domain1.com;
    access_log   logs/domain1.access.log  main;
    root         html;

    location ~ \.php$ {
      fastcgi_pass   127.0.0.1:1025;
    }
  }

  server { # simple reverse-proxy
    listen       80;
    server_name  domain2.com www.domain2.com;
    access_log   logs/domain2.access.log  main;

    # serve static files
    location ~ ^/(images|javascript|js|css|flash|media|static)/  {
      root    /var/www/virtual/big.server.com/htdocs;
      expires 30d;
    }

    # pass requests for dynamic content to rails/turbogears/zope, et al
    location / {
      proxy_pass      http://127.0.0.1:8080;
    }
  }

  upstream big_server_com {
    server 127.0.0.3:8000 weight=5;
    server 127.0.0.3:8001 weight=5;
    server 192.168.0.1:8000;
    server 192.168.0.1:8001;
  }

  server { # simple load balancing
    listen          80;
    server_name     big.server.com;
    access_log      logs/big.server.access.log main;

    location / {
      proxy_pass      http://big_server_com;
    }
  }
}
*/
