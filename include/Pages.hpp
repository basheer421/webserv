/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pages.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 15:15:00 by mkhan             #+#    #+#             */
/*   Updated: 2023/09/24 14:02:37 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <ServerTraits.hpp>

const	std::string	dirList(const std::string& path, const std::string& reqURL);
const	std::string	getErrPage(const std::string& code, const std::string& mssg);