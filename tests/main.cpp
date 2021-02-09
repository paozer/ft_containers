#ifdef NO_UNIT_TESTS

#include "../srcs/list/list.hpp"
#include <list>
#include <vector>

int main(void)
{

    int v[] = {1, 34, 5, 5, 66, 122};
    ft::list<int> mylist;
    std::list<int> mylist2 (v + 1, v + 3);
    std::cout << "size = " << mylist.size() << '\n';
    for (auto it = mylist.begin(); it != mylist.end(); ++it)
        std::cout << *it << " ";
    for (auto it = mylist2.begin(); it != mylist2.end(); ++it)
        std::cout << *it << " ";
    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#endif
