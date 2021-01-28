#include "srcs/list.hpp"
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

        mylist.push_front(1);
        mylist.push_front(30);
        mylist.push_front(23);
        //mylist.push_front(25);
        printlist(mylist, "mylist");
        std::cout << '\n';

       // mylist.reverse();
        for (ft::list<int>::size_type i = 0; i < mylist.size(); ++i) {
                mylist.splice(mylist.begin(), mylist, --mylist.end());
                printlist(mylist, "mylist");
        }

        std::cout << '\n';
        printlist(mylist, "mylist");
    }
    //{
    //    ft::list<int> mylist1 (10, 1);
    //    ft::list<int> mylist2 (5, 0);

    //    printlist(mylist1, "mylist1");
    //    printlist(mylist2, "mylist2");
    //    swap(mylist1, mylist2);
    //    printlist(mylist1, "mylist1");
    //    printlist(mylist2, "mylist2");
    //}
    return (0);
}
