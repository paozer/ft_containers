#ifdef NO_UNIT_TESTS

#include "../srcs/list/list.hpp"

int main(int ac, char **av, char **ev)
{
    ft::list<int> mylist;

    for (int i = 0; i < 10; ++i)
        mylist.push_front(i);
    std::cout << mylist.size() << '\n';
    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#endif
