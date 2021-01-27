#pragma once

#include "list_node.hpp"
#include <iterator>

namespace ft {

template <class T>
class list_iterator
{
    public:
        typedef T value_type;
        typedef std::ptrdiff_t difference_type;
        typedef T * pointer;
        typedef T & reference;
        typedef std::bidirectional_iterator_tag iterator_category;

        list_iterator (list_node<value_type> * node) : _node(node) {};

        bool operator==(const list_iterator<value_type> & other) { return (_node == other._node); }
        bool operator!=(const list_iterator<value_type> & other) { return (_node != other._node); }

        const value_type & operator*(void) { return (_node->_content); }

        const list_iterator<value_type> & operator++(void)
        {
            assert(_node != NULL);
            _node = _node->next;
            return (*this);
        }

        const list_iterator<value_type> operator++(int)
        {
            list_iterator<value_type> tmp = *this;
            _node = _node->next;
            return (tmp);
        }

        const list_iterator<value_type> & operator--(void)
        {
            _node = _node->prev;
            return (*this);
        }

        const list_iterator<value_type> operator--(int)
        {
            list_iterator<value_type> tmp = *this;
            _node = _node->prev;
            return (tmp);
        }

    private:
        list_node<value_type> * _node;

        list_iterator ();
};
}
