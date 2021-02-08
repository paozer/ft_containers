#ifdef NO_UNIT_TESTS

#include "../srcs/list/list.hpp"
#include <list>

int main(void)
{

    std::list<int> mylist (-1, 10);
    std::cout << "size = " << mylist.size() << '\n';
    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#endif
