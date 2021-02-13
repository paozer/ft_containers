#pragma once

#include "../list/list.hpp"

namespace ft {

template < class T, class Container = list<T> >
class stack
{

    public:
        typedef T value_type;
        typedef Container container_type;
        typedef size_t size_type;

    public:
        explicit stack (const container_type& cntr = container_type())
            : _cntr(cntr)
        {
        }

        ~stack()
        {
        }

        bool empty() const { return _cntr.empty(); }
        size_type size() const { return _cntr.size(); }
        value_type& top(void) { return _cntr.back(); }
        const value_type& top(void) const { return _cntr.back(); }
        void push (const value_type& val) { _cntr.push_back(val); }
        void pop(void) { _cntr.pop_back(); }

        bool operator== (const stack<T, Container>& rhs) { return _cntr == rhs._cntr; }
        bool operator< (const stack<T, Container>& rhs) { return _cntr < rhs._cntr; }
        bool operator!= (const stack<T, Container>& rhs) { return _cntr != rhs._cntr; }
        bool operator<= (const stack<T, Container>& rhs) { return _cntr <= rhs._cntr; }
        bool operator> (const stack<T, Container>& rhs) { return _cntr > rhs._cntr; }
        bool operator>= (const stack<T, Container>& rhs) { return _cntr >= rhs._cntr; }

    private:
        container_type _cntr;

}; // class stack

} // namespace ft
