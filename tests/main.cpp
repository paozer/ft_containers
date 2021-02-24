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

template <class T, class Alloc = std::allocator<T> >
class testClass
{
    public:
        typedef typename Alloc::reference reference;

        testClass()
        {
        }
};

int main(void)
{
    //compile under linux fails
    testClass<const int> tc;
    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#endif
