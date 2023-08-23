/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 15:14:46 by mkhan             #+#    #+#             */
/*   Updated: 2023/08/23 12:20:13 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>

std::string replace(std::string str)
{
	for (size_t pos = str.find('-'); pos != std::string::npos; pos = str.find('-'))
	{
		str.replace(pos, 1, "_");
	}
	return(str);
}

int main()
{
	std::cout << replace("hi-helklo-hweufhbsdf-sf") << std::endl;
	return(0);
}