#include "srcs/list.hpp"
#include <list>

int main(int ac, char **av, char **ev)
{
    {
        ft::list<int> list(10, 5);
        std::cout << list.size() << '\n';
        std::cout << list.empty() << '\n';
        for (int i = list.size(); i > 0; --i)
            list.pop_front();
        std::cout << list.size() << '\n';
        std::cout << list.empty() << '\n';
    }
    //{
    //    std::list<int> list(10, 55);
    //    for (std::list<int>::iterator it = list.begin();
    //         it != list.end();
    //         ++it) {
    //        std::cout << *it << '\n';
    //    }
    //}
    return (0);
}
