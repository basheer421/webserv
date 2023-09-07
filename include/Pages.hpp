/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pages.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 15:15:00 by mkhan             #+#    #+#             */
/*   Updated: 2023/09/06 14:29:45 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

const	std::string	dirList(std::string	path, std::string reqURL);
const	std::string	getErrPage(std::string code, std::string mssg);