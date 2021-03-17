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

int main(void)
{
    std::cout << "no_unit_tests" << std::endl;
    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#endif
