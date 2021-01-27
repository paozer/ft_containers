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
        std::cout << list.empty() << "\n\n";
    }
    {
        ft::list<int> list(10, 5);
        for (ft::list<int>::iterator it = list.begin(); it != list.end(); ++it)
            std::cout << *it << '\n';
        std::cout << '\n';
        for (int i = list.size(); i > 4; --i)
            list.pop_front();
        for (int i = 0; i < 9; ++i)
            list.push_front(i);
        for (ft::list<int>::iterator it = list.begin(); it != list.end(); ++it)
            std::cout << *it << '\n';
    }
    {
        ft::list<std::string> list(10);
        ft::list<std::string>::iterator it = list.end();
        ++it;
    }
    return (0);
}
