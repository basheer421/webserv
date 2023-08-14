/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pages.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 15:14:46 by mkhan             #+#    #+#             */
/*   Updated: 2023/08/14 15:49:39 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Pages.hpp"
#include <dirent.h>

const	std::string	dirList(std::string	path)
{
	std::string html = "<html>\n"
						"\t<head>\n"                                                                                  
						"\t\t<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\n"               
						"\t\t<title>Directory listing"
						" </title>\n"
                       "\t</head>\n"
                       "\t<body>\n"
                       "\t\t<h1>Directory listing"
					   " </h1>\n"
                       "\t\t<hr>\n"
                       "\t\t<ul>\n";
	DIR	*dirptr = opendir(path.c_str());
	if (dirptr == NULL)
		return	html + "\t\t<li>COULD NOT OPEN"
                      "DIRECTORY</ li>\n\t\t</ ul>\n\t\t<hr>\n\t</ body>\n</ html>\n ";
	struct dirent *dirElement = readdir(dirptr);
	while (dirElement)
	{
		const	std::string &filename = dirElement->d_name;
		html += "\t\t\t<li><a href=\"\">" + filename + "</a></li>\n";
		dirElement = readdir(dirptr);
	}
	closedir(dirptr);
	return html + "\t\t</ul>\n\t\t<hr>\n\t</body>\n</html>\n";
}