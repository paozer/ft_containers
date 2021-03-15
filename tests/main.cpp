#ifdef NO_UNIT_TESTS

#include "../srcs/list/list.hpp"
#include "../srcs/vector/vector.hpp"
#include "../srcs/stack/stack.hpp"
#include "../srcs/queue/queue.hpp"
#include "../srcs/map/map.hpp"
#include "../srcs/set/set.hpp"
#include "../srcs/multimap/multimap.hpp"
#include "../srcs/multiset/multiset.hpp"
#include "../srcs/deque/deque.hpp"

#include <iostream>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <chrono>

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
    //testClass<const int> tc;
    std::vector<int> v (10, 23);

    std::cout << *(3 - v.end()) << std::endl;
    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#endif
