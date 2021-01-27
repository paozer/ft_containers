NAME	=	main

CC		=	clang++

SRC		= 	main.cpp

#CFLAGS	=	-Wall -Wextra -Werror -Isrcs/
#CFLAGS	=	-Wall -Wextra -Isrcs/
CFLAGS	=	-Wall -Wextra -g3 -fsanitize=address -Isrcs/

all: 	$(NAME)

$(NAME):Makefile $(SRC) srcs/*
		@$(CC) $(CFLAGS) $(SRC) -o $(NAME)
		@echo "all your base are belong to us"

clean:
		@rm -rf main.o
		@rm -rf main.dSYM
		@echo "make clean done..."

fclean: clean
		@rm -rf $(NAME)
		@echo "make fclean done..."

re:		fclean all

.PHONY: all fclean clean re
