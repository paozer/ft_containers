NAME	=	test

CC		=	clang++

MAIN_SRC = 	tests/main.cpp

UNIT_TEST_SRC = tests/list.cpp

CFLAGS	=	-Wall -Wextra -std=c++11

CFLAGS_NO_UNIT_TESTS =	-Wall -Wextra -std=c++98

all: 	$(NAME)

$(NAME):Makefile $(SRC) $(UNIT_TEST_SRC) srcs/**
		@$(CC) $(CFLAGS) $(MAIN_SRC) $(UNIT_TEST_SRC) -o $(NAME)
		@echo "execute ./$(NAME) to run all unit tests"

no_unit_tests: Makefile $(SRC) srcs/**
		$(CC) $(CFLAGS_NO_UNIT_TESTS) -DNO_UNIT_TESTS $(MAIN_SRC) -o $(NAME)
		@echo "execute ./$(NAME) to run main specified in tests/main.cpp"

clean:
		@rm -rf $(NAME).o
		@rm -rf $(NAME).dSYM
		@echo "make clean done..."

fclean: clean
		@rm -rf $(NAME)
		@echo "make fclean done..."

re:		fclean all

.PHONY: all fclean clean re
