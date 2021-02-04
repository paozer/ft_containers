#pragma once

#include "list_node.hpp"
#include "utils.hpp" // choose<is_const, const, non_const>
#include <iterator>

namespace ft {

template <class T, bool is_const>
class list_iterator
{
    public:
        typedef T value_type;
        typedef std::ptrdiff_t difference_type;
        typedef typename choose<is_const, const T *, T *>::type pointer;
        typedef typename choose<is_const, const T &, T &>::type reference;
        typedef std::bidirectional_iterator_tag iterator_category;

    private:
        typedef list_iterator<value_type, is_const> self_type;
        typedef typename choose<is_const, const list_node<T> *, list_node<T> *>::type node_pointer;

    public:

        list_iterator (list_node<value_type> * node = NULL) : _node(node) {}

        list_iterator (const list_iterator<value_type, false>& other) : _node(other.get_node()) {}

        list_iterator &operator=(const self_type & other)
        {
            if (*this != other)
                _node = other._node;
            return *this;
        }

        bool operator==(const self_type & other) { return _node == other._node; }
        bool operator!=(const self_type & other) { return _node != other._node; }

        pointer operator->(void) { return &(_node->content); }
        reference operator*(void) { return _node->content; }

        self_type & operator++(void)
        {
            _node = _node->next;
            return *this;
        }

        self_type operator++(int)
        {
            self_type tmp = *this;
            _node = _node->next;
            return tmp;
        }

        self_type & operator--(void)
        {
            _node = _node->prev;
            return *this;
        }

        self_type operator--(int)
        {
            self_type tmp = *this;
            _node = _node->prev;
            return tmp;
        }

        node_pointer get_prev(void) const { return _node->prev; }
        node_pointer get_next(void) const { return _node->next; }
        node_pointer get_node(void) const { return _node; }

    private:
        node_pointer _node;

}; // class list_iterator
} // namespace ft
