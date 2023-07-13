/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bammar <bammar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 20:06:07 by bammar            #+#    #+#             */
/*   Updated: 2023/07/13 21:27:01 by bammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main(void)
{
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    /* htonl converts a long integer (e.g. address) to a network representation */
    /* htons converts a short integer (e.g. port) to a network representation */
    bzero((char *)&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);
    int server_fd = socket(address.sin_family, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        std::cerr << "cannot create socket\n";
        return (1);
    }
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "bind failed\n";
        return (1);
    }
    if (listen(server_fd, 10) < 0)
    {
        std::cerr << "listen failed\n";
        return (1);
    }

    while (true)
    {
        int client;

        std::cout << "Waiting for a connection...\n";
        client = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (client < 0)
        {
            std::cerr << "listen failed\n";
            return (1);
        }
        char buffer[30000] = {0};
        int read_res = read(client, buffer, 29999);
        if (read_res < 0)
        {
            std::cerr << "read failed\n";
            return (1);
        }
        std::string buffer_str = buffer;
        std::cout << buffer_str << std::endl;
        close(client);
    }
    return (0);
}
