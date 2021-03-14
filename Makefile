############## VARIABLES ##############

NAME	=	test

MAIN_SRC =	tests/main.cpp

TEST_SRC =	tests/map.cpp\
			tests/list.cpp\
			tests/deque.cpp\
			tests/vector.cpp\
			tests/stack_queue.cpp

CC		=	clang++

NO_UNIT_FLAGS =	-fsanitize=address -g3 -Wall -Wextra -std=c++98 -DNO_UNIT_TESTS

UNIT_FLAGS	=	-fsanitize=address -g3 -Wall -Wextra -Werror -std=c++2a

COVERAGE_FLAGS	=	--coverage -g -O0 -Wall -Wextra -Werror -std=c++2a

############## RECIPES ##############

all:		$(NAME)

$(NAME):	Makefile $(MAIN_SRC) $(TEST_SRC) srcs/**/*.hpp
			@echo "building unit tests..."
			@$(CC) $(UNIT_FLAGS) $(MAIN_SRC) $(TEST_SRC) -o $(NAME)
			@echo "execute ./$(NAME) to run all unit tests"

coverage:	Makefile $(MAIN_SRC) $(TEST_SRC) srcs/**/*.hpp
			@echo "building unit tests with coverage..."
			@$(CC) $(COVERAGE_FLAGS) $(MAIN_SRC) $(TEST_SRC) -o $(NAME)
			@echo "running executable to generate coverage data..."
			@./$(NAME)
			@echo "creating html report..."
			@gcovr --delete --root . --html --html-details -o coverage.html
			@mkdir -p COVERAGE_REPORT && mv *.html COVERAGE_REPORT
			@echo "report can be found in ./COVERAGE_REPORT/coverage.html"
			@make clean

no_unit_tests: Makefile $(MAIN_SRC) srcs/**
			$(CC) $(NO_UNIT_FLAGS) $(MAIN_SRC) -o $(NAME)
			@echo "execute ./$(NAME) to run main specified in tests/main.cpp"

clean:
			@rm -rf $(NAME).o
			@rm -rf $(NAME).dSYM
			@rm -f *.gcno
			@echo "make clean done..."

fclean:		clean
			@rm -rf $(NAME)
			@rm -rf COVERAGE_REPORT
			@echo "make fclean done..."

re:			fclean all

.PHONY:		all fclean clean re
