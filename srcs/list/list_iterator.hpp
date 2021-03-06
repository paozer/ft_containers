#pragma once

#include "list_node.hpp"
#include "../utils/utils.hpp"

#include <cstddef> // NULL, std::ptrdiff_t

#include <iterator> // std::bidirectional_iterator_tag

namespace ft {

template <class T, bool is_const>
class list_iterator
{
    public:
        typedef T value_type;
        typedef std::ptrdiff_t difference_type;
        typedef typename choose<is_const, const T*, T*>::type pointer;
        typedef typename choose<is_const, const T&, T&>::type reference;
        typedef std::bidirectional_iterator_tag iterator_category;

    private:
        typedef list_iterator<value_type, is_const> self_type;
        typedef typename choose<is_const, const list_node<value_type>*, list_node<value_type>*>::type node_pointer;

    public:
        /* CONSTRUCTORS */
        list_iterator (node_pointer node = NULL)
            : _node(node)
        {
        }

        list_iterator (const list_iterator<value_type, false>& other)
            : _node(other.get_node())
        {
        }

        /* OPERATORS */
        list_iterator& operator= (const self_type& other)
        {
            if (this != &other)
                _node = other._node;
            return *this;
        }

        friend bool operator== (const self_type& rhs, const self_type& lhs) { return rhs._node == lhs._node; }
        friend bool operator!= (const self_type& rhs, const self_type& lhs) { return rhs._node != lhs._node; }

        reference operator* (void) { return _node->content; }
        pointer operator-> (void) { return &_node->content; }

        self_type& operator++ (void)
        {
            _node = _node->next;
            return *this;
        }

        self_type operator++ (int)
        {
            self_type tmp = *this;
            _node = _node->next;
            return tmp;
        }

        self_type& operator-- (void)
        {
            _node = _node->prev;
            return *this;
        }

        self_type operator-- (int)
        {
            self_type tmp = *this;
            _node = _node->prev;
            return tmp;
        }

        /* GETTERS */
        node_pointer get_prev (void) const { return _node->prev; }
        node_pointer get_next (void) const { return _node->next; }
        node_pointer get_node (void) const { return _node; }

    private:
        node_pointer _node;

}; // CLASS LIST_ITERATOR

template <class T, bool is_const>
class reverse_list_iterator
{
    public:
        typedef T value_type;
        typedef std::ptrdiff_t difference_type;
        typedef typename choose<is_const, const T*, T*>::type pointer;
        typedef typename choose<is_const, const T&, T&>::type reference;
        typedef std::bidirectional_iterator_tag iterator_category;

    private:
        typedef reverse_list_iterator<value_type, is_const> self_type;
        typedef typename choose<is_const, const list_node<value_type>*, list_node<value_type>*>::type node_pointer;

    public:
        /* CONSTRUCTORS */
        reverse_list_iterator (node_pointer node = NULL)
            : _node(node)
        {
        }

        reverse_list_iterator (const reverse_list_iterator<value_type, false>& other)
            : _node(other.get_node())
        {
        }

        /* OPERATORS */
        reverse_list_iterator& operator= (const self_type& other)
        {
            if (this != &other)
                _node = other._node;
            return *this;
        }

        friend bool operator== (const self_type& rhs, const self_type& lhs) { return rhs._node == lhs._node; }
        friend bool operator!= (const self_type& rhs, const self_type& lhs) { return !(rhs == lhs); }

        reference operator* (void) { return _node->content; }
        pointer operator-> (void) { return &_node->content; }

        self_type& operator++ (void)
        {
            _node = _node->prev;
            return *this;
        }

        self_type operator++ (int)
        {
            self_type tmp = *this;
            _node = _node->prev;
            return tmp;
        }

        self_type& operator-- (void)
        {
            _node = _node->next;
            return *this;
        }

        self_type operator-- (int)
        {
            self_type tmp = *this;
            _node = _node->next;
            return tmp;
        }

        /* GETTERS */
        node_pointer get_prev (void) const { return _node->prev; }
        node_pointer get_next (void) const { return _node->next; }
        node_pointer get_node (void) const { return _node; }

    private:
        node_pointer _node;

}; // CLASS REVERSE_LIST_ITERATOR

} // NAMESPACE FT
