#ifdef NO_UNIT_TESTS

#include "../srcs/list/list.hpp"
#include "../srcs/vector/vector.hpp"
#include "../srcs/stack/stack.hpp"
#include "../srcs/queue/queue.hpp"
#include "../srcs/map/map.hpp"
#include "../srcs/utils/llrb_tree.hpp"

#include <list>
#include <vector>
#include <map>


template < class K, class T, class Compare = std::less<K> >
struct comp_s {
    bool operator()(const std::pair<K, T>& p, const std::pair<K, T>& s)
    {
        Compare comp = Compare();
        return (comp(p.first, s.first));
    }
};

int main(void)
{
    {
        ft::map<int, int> rb;
        for (int i (0); i < 10; ++i) {
            int key = rand();
            int val = rand();
            std::cout << "inserted key [" << key << "] val [" << i << "]\n";
            rb.insert(std::make_pair(key, i));
        }
        std::cout << "\n";
        for (auto it = rb.begin(); it != rb.end(); ++it)
            std::cout << "key [" << it->first << "] val [" << it->second << "]\n";
        std::cout << "size [" << rb.size() << "]\n";
        std::cout << "\nbegin key [" << rb.begin()->first << "] val [" << rb.begin()->second << "]\n";
        std::cout << "end key [" << rb.end()->first << "] val [" << rb.end()->second << "]\n";
    }
    //{
    //    ft::LLRB<std::pair<int, int>, comp_s<int, int> > rb ;
    //    for (int i (0); i < 1; ++i)
    //        rb.insert(std::make_pair(rand() % 100, rand()));
    //    rb.insert(std::make_pair(40, rand()));
    //    rb.insert(std::make_pair(40, rand()));
    //    rb.insert(std::make_pair(40, rand()));
    //    rb.print_tree();
    //    rb.print_min_max();
    //    std::cout << rb.size() << " size\n";
    //    std::cout << "\nclearing the tree\n";
    //    rb.clear();
    //    rb.print_tree();
    //    rb.print_min_max();
    //    std::cout << rb.size() << " size\n";
    //}
    //{
    //    std::map<int, int> m;

    //    for (int i (0); i < 10; ++i) {
    //        m.insert(std::make_pair(i, rand() % 70));
    //    }
    //    for (auto it = m.begin(); it != m.end(); ++it) {
    //        std::cout << "key " << it->first << " value " << it->second << std::endl;
    //    }
    //}
    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#endif
