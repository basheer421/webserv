# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bammar <bammar@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/13 20:01:42 by bammar            #+#    #+#              #
#    Updated: 2023/07/23 02:06:27 by bammar           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	webserv

SRCS_FOLDER = ./src

INCLUDE_FOLDERS = -I./include -I./lib/string

FILES_NAMES = main.cpp Server.cpp ParserConf.cpp http_special_response.cpp Request.cpp

FILES = lib/string/ft_string.cpp $(addprefix $(SRCS_FOLDER)/,$(FILES_NAMES)) 

CXX		=	c++

CXXFLAGS =	-Wall -Wextra -Werror -std=c++98

OBJS	=	$(FILES:.cpp=.o)

all: $(NAME)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE_FOLDERS) -c $< -o $@

$(NAME): $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) $(INCLUDE_FOLDERS) -o $(NAME)

clean:
	/bin/rm -f $(OBJS)

fclean:	clean
	/bin/rm -f $(NAME)

re:	fclean all

.PHONY:	all clean fclean re
