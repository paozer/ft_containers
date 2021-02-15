#ifdef NO_UNIT_TESTS

#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <map>

#include "../srcs/list/list.hpp"
#include "../srcs/vector/vector.hpp"
#include "../srcs/stack/stack.hpp"
#include "../srcs/queue/queue.hpp"
#include "../srcs/map/map.hpp"

int main(void)
{
    {
         ft::map<int, int> rb;
         size_t insert_count (0);
         for (int i (0); i < 50; ++i) {
             int key = rand() % 100;
             ++insert_count;
             rb.insert(std::make_pair(key, i));
         }
         size_t iter_count (0);
         for (auto it = rb.begin(); it != rb.end(); ++it) {
             std::cout << "key [" << it->first << "] val [" << it->second << "]\n";
             ++iter_count;
         }
         std::cout << "\n";
         for (auto it = rb.rbegin(); it != rb.rend(); ++it) {
             std::cout << "key [" << it->first << "] val [" << it->second << "]\n";
         }

         std::cout << "--end() " << (--rb.end())->first << "\n";
         std::cout << "\ninsert vs iter count [" << insert_count << "] [" << iter_count << "]\n";
    }
    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#endif
