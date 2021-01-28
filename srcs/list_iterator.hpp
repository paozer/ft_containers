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

        list_iterator () : _node(NULL) {} // private ?
        list_iterator (list_node<value_type> * node) : _node(node) {};
        //~list_iterator () { delete _node; }
        list_iterator &operator=(const list_iterator & other)
        {
            if (*this != other)
                _node = other._node;
            return (*this);
        }

        bool operator==(const list_iterator<value_type> & other) { return (_node == other._node); }
        bool operator!=(const list_iterator<value_type> & other) { return (_node != other._node); }

        const value_type & operator*(void) { return (_node->content); }

        const list_iterator<value_type> & operator++(void)
        {
            //assert(_node != NULL); //?
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

        list_node<value_type> * get_prev(void) { return (_node->prev); }
        list_node<value_type> * get_next(void) { return (_node->next); }
        list_node<value_type> * get_node(void) { return (_node); }

    private:
        list_node<value_type> * _node;
};
}
