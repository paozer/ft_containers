#ifdef NO_UNIT_TESTS

#include "../srcs/list/list.hpp"
#include "../srcs/vector/vector.hpp"
#include "../srcs/stack/stack.hpp"
#include "../srcs/queue/queue.hpp"
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
        ft::LLRB<std::pair<int, int>, comp_s<int, int> > rb ;
        for (int i (0); i < 1; ++i)
            rb.insert(std::make_pair(rand() % 100, rand()));
        rb.insert(std::make_pair(40, rand()));
        rb.insert(std::make_pair(40, rand()));
        rb.insert(std::make_pair(40, rand()));
        rb.print_tree();
        rb.print_min_max();
        std::cout << rb.size() << " size\n";
        std::cout << "\nclearing the tree\n";
        rb.clear();
        rb.print_tree();
        rb.print_min_max();
        std::cout << rb.size() << " size\n";
    }
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
