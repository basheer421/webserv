/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pages.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 15:14:46 by mkhan             #+#    #+#             */
/*   Updated: 2023/08/16 12:34:14 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Pages.hpp"
#include <dirent.h>

const	std::string	dirList(std::string	path)
{
	std::string html = "<html>"
						"<head>"                                                                                  
						"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">"               
						"<title>Directory listing"
						" </title>"
                       "</head>"
                       "<body>"
                       "<h1>Directory listing"
					   " </h1>"
                       "<hr>"
                       "<ul>";
	DIR	*dirptr = opendir(path.c_str());
	if (dirptr == NULL)
		return	html + "<li>COULD NOT OPEN"
                      "DIRECTORY</ li></ ul><hr></ body></ html> ";
	struct dirent *dirElement = readdir(dirptr);
	while (dirElement)
	{
		const	std::string &filename = dirElement->d_name;
		html += "<li><a href=\"\">" + filename + "</a></li>";
		dirElement = readdir(dirptr);
	}
	closedir(dirptr);
	return html + "</ul><hr></body></html>";
}