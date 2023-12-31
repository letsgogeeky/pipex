# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/12 20:26:46 by ramoussa          #+#    #+#              #
#    Updated: 2023/08/30 01:52:15 by ramoussa         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME:= pipex

CFLAGS	:= -Wextra -Wall -Werror -g -fsanitize=address
BASELIB := ./lib/ft-baselib

HEADERS := -I ./include -I ${BASELIB}/include
LIBS := ${BASELIB}/baselib.a

SRCS := path.c sanitizer.c helper.c file_manager.c here_doc.c main.c
OBJS := ${addprefix src/, ${SRCS:.c=.o}}

all: BASELIB ${NAME}

%.o: %.c
	@$(CC) $(CFLAGS) $(HEADERS) -o $@ -c $<

${NAME}: ${OBJS}
	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(HEADERS) -o $(NAME) && echo "Successful build...!"

BASELIB:
	@if [ -d ${BASELIB} ]; then\
		echo "${BASELIB} already exists... proceeding to next step.";\
	else\
		mkdir lib;\
		git clone git@github.com:letsgogeeky/ft-baselib.git ${BASELIB};\
	fi
	make --directory=${BASELIB}

clean:
	@rm -rf $(OBJS)

fclean: clean
	make fclean --directory=${BASELIB}
	rm -f ${NAME}

re: fclean all

.PHONY: all rmlib clean fclean re