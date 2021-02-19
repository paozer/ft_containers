#pragma once

#include "../list/list.hpp"
#include <cstddef> // size_t

namespace ft {

template < class T, class Container = ft::list<T> >
class queue
{
    public:
        typedef T value_type;
        typedef Container container_type;
        typedef size_t size_type;

        explicit queue (const container_type& cntr = container_type())
            : _cntr(cntr) {}

        ~queue() {}

        bool empty() const { return _cntr.empty(); }
        size_type size() const { return _cntr.size(); }

        value_type& front() { return _cntr.front(); }
        value_type& back() { return _cntr.back(); }
        const value_type& front() const { return _cntr.front(); }
        const value_type& back() const { return _cntr.back(); }

        void push (const value_type& val) { _cntr.push_back(val); }
        void pop(void) { _cntr.pop_front(); }

        bool operator== (const queue<T, Container>& rhs) { return _cntr == rhs._cntr; }
        bool operator< (const queue<T, Container>& rhs) { return _cntr < rhs._cntr; }
        bool operator!= (const queue<T, Container>& rhs) { return _cntr != rhs._cntr; }
        bool operator<= (const queue<T, Container>& rhs) { return _cntr <= rhs._cntr; }
        bool operator> (const queue<T, Container>& rhs) { return _cntr > rhs._cntr; }
        bool operator>= (const queue<T, Container>& rhs) { return _cntr >= rhs._cntr; }

    private:
        container_type _cntr;

}; // CLASS QUEUE

} // NAMESPACE FT
