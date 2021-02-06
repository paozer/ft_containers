#include "srcs/list/list.hpp"
#include <list>

template <typename t>
void printlist(t & list, const std::string & name)
{
    std::cout << name << " size [" << list.size() << "]\t";
    for (typename t::iterator it = list.begin(); it != list.end(); ++it) {
        std::cout << " [" << *it << "]";
    }
    std::cout << '\n';
}

int main(int ac, char **av, char **ev)
{
    {
        ft::list<int> mylist;
        for (int i=1; i<=5; ++i) mylist.push_back(i);
        ft::list<int>::reverse_iterator it = mylist.rbegin();
        ft::list<int>::const_reverse_iterator rcit (it);

        std::cout << "mylist backwards:";
        for (ft::list<int>::reverse_iterator rit = mylist.rbegin(); rit != mylist.rend(); ++rit)
            std::cout << ' ' << *rit;
    }
    return (0);
}
