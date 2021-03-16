#pragma once

#include "../deque/deque.hpp"
#include <cstddef> // size_t

namespace ft {

template < class T, class Container = ft::deque<T> >
class stack
{
    public:
        typedef T value_type;
        typedef Container container_type;
        typedef size_t size_type;

        explicit stack (const container_type& container = container_type())
            : c(container)
        {
        }

        ~stack()
        {
        }

        bool empty() const { return c.empty(); }
        size_type size() const { return c.size(); }

        value_type& top(void) { return c.back(); }
        const value_type& top(void) const { return c.back(); }

        void push (const value_type& val) { c.push_back(val); }
        void pop(void) { c.pop_back(); }

        friend bool operator== (const stack<T, Container>& rhs, const stack<T, container_type> lhs) { return rhs.c == lhs.c; }
        friend bool operator< (const stack<T, Container>& rhs, const stack<T, container_type> lhs) { return rhs.c < lhs.c; }
        friend bool operator!= (const stack<T, Container>& rhs, const stack<T, container_type> lhs) { return rhs.c != lhs.c; }
        friend bool operator<= (const stack<T, Container>& rhs, const stack<T, container_type> lhs) { return rhs.c <= lhs.c; }
        friend bool operator> (const stack<T, Container>& rhs, const stack<T, container_type> lhs) { return rhs.c > lhs.c; }
        friend bool operator>= (const stack<T, Container>& rhs, const stack<T, container_type> lhs) { return rhs.c >= lhs.c; }

    protected:
        container_type c;

}; // CLASS STACK

} // NAMESPACE FT
