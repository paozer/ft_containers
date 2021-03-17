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

all:		$(NAME)

# compile program with unit tests
$(NAME):		Makefile $(MAIN_SRC) $(TEST_SRC) srcs/**/*.hpp
				@echo "building unit tests..."
				@$(CC) $(UNIT_FLAGS) $(MAIN_SRC) $(TEST_SRC) -o $(NAME)
				@echo "execute ./$(NAME) to run all unit tests"

# compile program with unit tests and stl container to verify test correctness
verify_tests:	Makefile $(MAIN_SRC) $(TEST_SRC) srcs/**/*.hpp
				@echo "building unit tests with stl containers..."
				@$(CC) $(UNIT_FLAGS) -DVERIFY_UNIT_TESTS $(MAIN_SRC) $(TEST_SRC) -o $(NAME)
				@echo "execute ./$(NAME) to run all unit tests"

# compile program with in tests/main.cpp i.e no unit tests
no_unit_tests: 	Makefile $(MAIN_SRC) srcs/**
				$(CC) $(NO_UNIT_FLAGS) $(MAIN_SRC) -o $(NAME)
				@echo "execute ./$(NAME) to run main specified in tests/main.cpp"

# compile program with coverage info and create html report
coverage:		Makefile $(MAIN_SRC) $(TEST_SRC) srcs/**/*.hpp
				@echo "building unit tests with coverage..."
				@$(CC) $(COVERAGE_FLAGS) $(MAIN_SRC) $(TEST_SRC) -o $(NAME)
				@echo "running executable to generate coverage data..."
				@./$(NAME)
				@echo "creating html report..."
				@gcovr --delete --root . --html --html-details -o coverage.html
				@mkdir -p COVERAGE_REPORT && mv *.html COVERAGE_REPORT
				@echo "report can be found in ./COVERAGE_REPORT/coverage.html"
				@make clean

# build and run valgrind docker container
valgrind:
				@echo "building container..."
				@docker build --rm --tag valgrind . >/dev/null
				@docker run --name valgrind -v $(PWD):/root/build -w /root/build -i -t valgrind bash

clean:
				@rm -rf $(NAME).o
				@rm -rf $(NAME).dSYM
				@rm -f *.gcno
				@docker rm --force valgrind 2&>/dev/null
				@echo "make clean done..."

fclean:			clean
				@rm -rf $(NAME)
				@rm -rf COVERAGE_REPORT
				@echo "make fclean done..."

re:				fclean all

.PHONY:		all fclean clean re
