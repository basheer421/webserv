/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserConf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 12:05:54 by bammar            #+#    #+#             */
/*   Updated: 2023/07/22 14:47:29 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParserConf.hpp"
#include <iostream>

ParserConf::ParserConf() {}

ParserConf::ParserConf(std::string& text) : text(text)
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

bool ParserConf::isModuleName(std::string& str)
{
	return (*(str.begin()) == '[' && *(str.end() - 1) == ']');
}

static std::vector<std::string> split(std::string& str, char sep)
{
	std::vector<std::string> vec;
	std::stringstream ss(str);
	std::string buff;

	while (std::getline(ss, buff, sep))
  {
    if (buff.length() == 0)
      continue ;
    vec.push_back(buff);
  }
	return vec;
}

/**
 * TODO:
 * - Replace all space chars (except '\\n') with '\ ' (space).
 * - Remove everything after ';' (comments).
 * - Make your exceptions.
*/
std::vector<ParserConf::Module> ParserConf::parseFile()
{
	std::vector<ParserConf::Module> file;

  std::vector<std::string> lines = split(text, '\n');
  
  for (std::vector<std::string>::iterator it = lines.begin();
    it < lines.end(); ++it)
  {
    std::string& line = *it;
    if (isModuleName(line))
    {
      Module modl;
      modl.name = line.substr(1, line.length() - 2);
      file.push_back(modl);
    }
    else
    {
      std::vector<std::string> segments = split(line, ' ');
      Directive dir;

      /*
        First element should always be a Module.
      */
      if (file.size() == 0)
        throw std::exception();
      
      /*
        There should be a name at the start.
      */
      if (segments.size() < 2)
        throw std::exception();
      
      dir.first = segments[0];
      segments.erase(segments.begin());
      dir.second = segments;
      
      file[file.size() - 1].directives.push_back(dir);
    }
  }
	return (file);
}

void ParserConf::printDirective(const Directive& dir)
{
	std::cout << dir.first << " ";
	for (std::vector<std::string>::const_iterator dit2 =
		dir.second.begin();
		dit2 != dir.second.end();
		++dit2)
	{
		std::cout << "{" << *dit2  << "}" << (dit2 + 1 == dir.second.end() ? "" : " ");
	}
	std::cout << "\n";
}

void ParserConf::print(const std::vector<ParserConf::Module>& conf)
{
	for (std::vector<ParserConf::Module>::const_iterator it = conf.begin();
		it != conf.end(); ++it)
	{
		std::cout << "[" << (*it).name << "]\n";

		for (std::vector<Directive>::const_iterator dit =
			(*it).directives.begin();
			dit != (*it).directives.end();
			++dit)
		{
			printDirective(*dit);
		}
	}
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
