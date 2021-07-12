############## VARIABLES ##############

NAME	=	test

MAIN_SRC =	tests/main.cpp

TEST_SRC =	tests/map.cpp\
			tests/list.cpp\
			tests/deque.cpp\
			tests/vector.cpp\
			tests/set.cpp\
			tests/multi.cpp\
			tests/stack_queue.cpp

CC		=	clang++

NO_UNIT_FLAGS =	-fsanitize=address -g3 -Wall -Wextra -std=c++2a -DNO_UNIT_TESTS

UNIT_FLAGS	=	-fsanitize=address -g3 -Wall -Wextra -std=c++2a

COVERAGE_FLAGS	=	--coverage -g -O0 -Wall -Wextra -std=c++2a

############## RECIPES ##############

# compile program with unit tests
$(NAME):		Makefile $(MAIN_SRC) $(TEST_SRC) srcs/**/*.hpp
				@echo "\033[33m > building unit tests...\033[0m"
				@$(CC) $(UNIT_FLAGS) $(MAIN_SRC) $(TEST_SRC) -o $(NAME)
				@echo "\033[32m > unit tests compiled\033[0m"
				@echo "\033[32m > execute ./$(NAME) to run all unit tests\033[0m"

# compile program with unit tests and stl container to verify test correctness
verify_tests:	Makefile $(MAIN_SRC) $(TEST_SRC) srcs/**/*.hpp
				@echo "\033[33m > building unit tests with stl containers...\033[0m"
				@$(CC) $(UNIT_FLAGS) -DVERIFY_UNIT_TESTS $(MAIN_SRC) $(TEST_SRC) -o $(NAME)
				@echo "\033[32m > unit tests compiled\033[0m"
				@echo "\033[32m > execute ./$(NAME) to run all unit tests\033[0m"

# compile program with in tests/main.cpp i.e no unit tests
no_unit_tests: 	Makefile $(MAIN_SRC) srcs/**
				@echo "\033[33m > building program using test/main.cpp...\033[0m"
				$(CC) $(NO_UNIT_FLAGS) $(MAIN_SRC) -o $(NAME)
				@echo "\033[32m > execute ./$(NAME) to run main specified in tests/main.cpp\033[0m"

# compile program with coverage info and create html report
coverage:		Makefile $(MAIN_SRC) $(TEST_SRC) srcs/**/*.hpp
				@echo "\033[33m > building unit tests w/ test coverage analysis...\033[0m"
				@$(CC) $(COVERAGE_FLAGS) $(MAIN_SRC) $(TEST_SRC) -o $(NAME)
				@echo "\033[33m > generating coverage data...\033[0m"
				@./$(NAME) > /dev/null
				@echo "\033[33m > creating html report...\033[0m"
				@gcovr --delete --root . --html --html-details -o coverage.html
				@mkdir -p COVERAGE_REPORT && mv *.html COVERAGE_REPORT
				@echo "\033[32m > report can be found in ./COVERAGE_REPORT/coverage.html\033[0m"
				@make clean > /dev/null

# build and run valgrind docker container
valgrind:
				@echo "\033[33m > building valgrind container...\033[0m"
				@docker build --rm --tag valgrind . >/dev/null
				@docker run --name valgrind -v $(PWD):/root/build -w /root/build -i -t valgrind bash

clean:
				@rm -rf $(NAME).o
				@rm -rf $(NAME).dSYM
				@rm -f *.gcno
				@echo "\033[31m > make clean done\033[0m"

fclean:			clean
				@rm -rf $(NAME)
				@rm -rf COVERAGE_REPORT
				@echo "\033[31m > make fclean done\033[0m"

re:				fclean $(NAME)
