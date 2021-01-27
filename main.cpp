#include "srcs/list.hpp"
#include <list>

int main(int ac, char **av, char **ev)
{
    {
        ft::list<int> list;
        std::cout << "list size " << list.size() << "\n";
        for (int i = 0; i < 10; ++i)
            list.push_front(i * 10);
        std::cout << "list size " << list.size() << "\n";
        for (ft::list<int>::iterator it = list.begin(); it != list.end(); ++it)
            std::cout << "[" << *it << "] ";
        std::cout << "\n";
        for (int i = 0; i < 10; ++i)
            list.push_back(i * 10);
        std::cout << "list size " << list.size() << "\n";
        for (ft::list<int>::iterator it = list.begin(); it != list.end(); ++it)
            std::cout << "[" << *it << "] ";
        std::cout << "\n";
    }
    //{
    //    ft::list<int> list(10, 5);
    //    for (ft::list<int>::iterator it = list.begin(); it != list.end(); ++it)
    //        std::cout << *it << '\n';
    //    std::cout << '\n';
    //    for (int i = list.size(); i > 4; --i)
    //        list.pop_front();
    //    for (int i = 0; i < 9; ++i)
    //        list.push_front(i);
    //    for (ft::list<int>::iterator it = list.begin(); it != list.end(); it++)
    //        std::cout << *it << '\n';
    //}
    //{
    //    std::cout << "\n";
    //    ft::list<int> list;
    //    for (int i = 0; i < 9; ++i)
    //        list.push_front(i);
    //    for (ft::list<int>::iterator it = list.begin(); it != list.end(); it++)
    //        std::cout << *it << '\n';
    //    std::cout << "\n";
    //    for (ft::list<int>::iterator it = list.end(); it != list.begin(); --it)
    //        std::cout << *it << '\n';
    //    std::cout << "\n";
    //    std::cout << *(list.begin()) << "\n";
    //    ft::list<int>::iterator end = list.end();
    //    --end;
    //    std::cout << *end << "\n";
    //}
    return (0);
}
