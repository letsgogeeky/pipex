NAME:= pipex

CFLAGS	:= -Wextra -Wall -Werror -g -O3 -funroll-loops -flto
BASELIB := ./lib/ft-baselib

HEADERS := -I ./include -I ${BASELIB}/include
LIBS := ${BASELIB}/baselib.a

SRCS := main.c
OBJS := ${addprefix src/, ${SRCS:.c=.o}}

all: BASELIB ${NAME}

%.o: %.c
	@$(CC) $(CFLAGS) $(HEADERS) -o $@ -c $<

${NAME}: ${OBJS}
	@$(CC) $(OBJS) $(LIBS) $(HEADERS) -o $(NAME) && echo "Successful build...!"

BASELIB:
	make --directory=${BASELIB}

clean:
	@rm -rf $(OBJS)

fclean: clean
	make fclean --directory=${BASELIB}
	rm -f ${NAME}

re: fclean all

.PHONY: all rmlib clean fclean re