NAME	=	test

CC		=	clang++

MAIN_SRC =	tests/main.cpp

TEST_SRC =	tests/map.cpp\
			tests/list.cpp\
			tests/deque.cpp\
			tests/vector.cpp\
			tests/stack_queue.cpp

CFLAGS	=	-fsanitize=address -g3 -Wall -Wextra -Werror -std=c++2a

CFLAGS_ =	-fsanitize=address -g3 -Wall -Wextra -std=c++2a

all:		$(NAME)

$(NAME):	Makefile $(MAIN_SRC) $(TEST_SRC) srcs/**
			@$(CC) $(CFLAGS) $(MAIN_SRC) $(TEST_SRC) -o $(NAME)
			@echo "execute ./$(NAME) to run all unit tests"

no_unit_tests: Makefile $(MAIN_SRC) srcs/**
			$(CC) $(CFLAGS_) -DNO_UNIT_TESTS $(MAIN_SRC) -o $(NAME)
			@echo "execute ./$(NAME) to run main specified in tests/main.cpp"

clean:
			@rm -rf $(NAME).o
			@rm -rf $(NAME).dSYM
			@echo "make clean done..."

fclean:		clean
			@rm -rf $(NAME)
			@echo "make fclean done..."

re:			fclean all

.PHONY:		all fclean clean re
