#pragma once

#include "avl_node.hpp"
#include "utils.hpp"

#include <cstddef> // std.:ptrdiff_t, NULL
#include <iterator> // std::bidirectional_iterator_tag

namespace ft {

template <class T, bool is_const>
class avl_iterator
{
    public:
        typedef T value_type;
        typedef std::ptrdiff_t difference_type;
        typedef typename choose<is_const, const value_type*, value_type*>::type pointer;
        typedef typename choose<is_const, const value_type&, value_type&>::type reference;
        typedef std::bidirectional_iterator_tag iterator_category;

    private:
        typedef avl_iterator<value_type, is_const> self_type;
        typedef typename choose<is_const, const avl_node<value_type>*, avl_node<value_type>*>::type node_pointer;

    public:
        /* CONSTRUCTORS */
        avl_iterator (avl_node<value_type>* node = NULL)
            : _node(node)
        {
        }

        // need GETTER to node because if non-const_it to const_it the classes are not
        // the same and we can't access private members
        avl_iterator (const avl_iterator<value_type, false>& other)
            : _node(other.get_node())
        {
        }

        /* OPERATORS */
        avl_iterator& operator= (const self_type& other)
        {
            if (this != &other)
                _node = other._node;
            return *this;
        }

        friend bool operator== (const self_type& rhs, const self_type& lhs) { return rhs._node == lhs._node; }
        friend bool operator!= (const self_type& rhs, const self_type& lhs) { return rhs._node != lhs._node; }

        reference operator* (void) { return _node->content; }
        pointer operator-> (void) { return &_node->content; }

        /* if there is a right subtree goto it's smallest node
         * else if we are done w/ left subtree go to subtree root
         * else if we are done w/ right subtree go to subtree root parent
         */
        self_type& operator++ (void)
        {
            node_pointer parent = _node->parent;
            if (_node->right) {
                _node = _node->right;
                while (_node->left)
                    _node = _node->left;
            }
            else if (_node == parent->left)
                _node = parent;
            else {
                while (_node == parent->right) {
                    _node = parent;
                    parent = _node->parent;
                }
                _node = _node->parent;
            }
            return *this;
        }

        self_type operator++ (int)
        {
            self_type tmp = *this;
            ++*this;
            return tmp;
        }

        self_type& operator-- (void)
        {
            node_pointer parent = _node->parent;
            if (_node->left) {
                _node = _node->left;
                while (_node->right)
                    _node = _node->right;
            }
            else if (_node == parent->right)
                _node = parent;
            else {
                while (_node == parent->left) {
                    _node = parent;
                    parent = _node->parent;
                }
                _node = _node->parent;
            }
            return *this;
        }

        self_type operator-- (int)
        {
            self_type tmp = *this;
            --*this;
            return tmp;
        }

        /* GETTER */
        node_pointer get_node (void) const { return _node; }

    private:
        node_pointer _node;

}; // CLASS AVL_ITERATOR

template <class T, bool is_const>
class reverse_avl_iterator
{
    public:
        typedef T value_type;
        typedef std::ptrdiff_t difference_type;
        typedef typename choose<is_const, const value_type*, value_type*>::type pointer;
        typedef typename choose<is_const, const value_type&, value_type&>::type reference;
        typedef std::bidirectional_iterator_tag iterator_category;

    private:
        typedef reverse_avl_iterator<T, is_const> self_type;
        typedef typename choose<is_const, const avl_node<value_type>*, avl_node<value_type>*>::type node_pointer;

    public:
        /* CONSTRUCTORS */
        reverse_avl_iterator (node_pointer node = NULL)
            : _node(node)
        {
        }

        reverse_avl_iterator (const reverse_avl_iterator<value_type, false>& other)
            : _node(other.get_node())
        {
        }

        /* OPERATORS */
        reverse_avl_iterator& operator= (const self_type& other)
        {
            if (this != &other)
                _node = other._node;
            return *this;
        }

        friend bool operator== (const self_type& rhs, const self_type& lhs) { return rhs._node == lhs._node; }
        friend bool operator!= (const self_type& rhs, const self_type& lhs) { return rhs._node != lhs._node; }

        reference operator* (void) { return _node->content; }
        pointer operator-> (void) { return &_node->content; }

        /* if there is a right subtree goto it's smallest node
         * else if we are done w/ left subtree go to subtree root
         * else if we are done w/ right subtree go to subtree root parent
         */
        self_type& operator++ (void)
        {
            node_pointer parent = _node->parent;
            if (_node->left) {
                _node = _node->left;
                while (_node->right)
                    _node = _node->right;
            }
            else if (_node == parent->right)
                _node = parent;
            else {
                while (_node == parent->left) {
                    _node = parent;
                    parent = _node->parent;
                }
                _node = _node->parent;
            }
            return *this;
        }

        self_type operator++ (int)
        {
            self_type tmp = *this;
            ++*this;
            return tmp;
        }

        self_type& operator-- (void)
        {
            node_pointer parent = _node->parent;
            if (_node->right) {
                _node = _node->right;
                while (_node->left)
                    _node = _node->left;
            }
            else if (_node == parent->left)
                _node = parent;
            else {
                while (_node == parent->right) {
                    _node = parent;
                    parent = _node->parent;
                }
                _node = _node->parent;
            }
            return *this;
        }

        self_type operator-- (int)
        {
            self_type tmp = *this;
            --*this;
            return tmp;
        }

        /* GETTER */
        node_pointer get_node (void) const { return _node; }

    private:
        node_pointer _node;

}; // CLASS REVERSE_AVL_ITERATOR

}; // NAMESPACE FT
