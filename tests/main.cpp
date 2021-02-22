#ifdef NO_UNIT_TESTS

#include "../srcs/list/list.hpp"
#include "../srcs/vector/vector.hpp"
#include "../srcs/stack/stack.hpp"
#include "../srcs/queue/queue.hpp"
#include "../srcs/map/map.hpp"
#include "../srcs/utils/rb_tree.hpp"
#include "../srcs/utils/avl_tree.hpp"

#include <iostream>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <map>

int main(void)
{
    ft::list<int> mylist (3, 4);
    ft::list<int>::iterator it = mylist.begin();
    ft::list<int>::const_iterator cit = mylist.begin();
    if (it == cit)
        std::cout << "YES" << std::endl;
    if (!(cit != it))
        std::cout << "YES" << std::endl;
    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#endif
