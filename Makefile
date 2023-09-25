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

CPU_USAGE = 80

SRCS_FOLDER = ./src

INCLUDE_FOLDERS = -I./include -I./lib/string

FILES_NAMES = main.cpp ParserConf.cpp ServerTraits.cpp Server.cpp \
	ServerManager.cpp Request.cpp Pages.cpp Response.cpp Cgi.cpp \

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

run_limited: all
	cpulimit -l $(CPU_USAGE) ./$(NAME) -f

.PHONY:	all clean fclean reb
