##
## EPITECH PROJECT, 2023
## my_ftp
## File description:
## Makefile
##

SRC 	=	process_server.c	\
			sever_suite.c	\
			command_suit.c	\
			command_suit2.c

NAME 	=	myftp


all: $(NAME)

$(NAME):
	gcc $(SRC) -o $(NAME) -g3

clean:
	rm -f *#
	rm -f *~
	rm -f *.gcda

fclean: clean
	rm -f $(NAME)
	rm -f $(NAME_TEST)
	rm -f *.gcno

re:	fclean all
