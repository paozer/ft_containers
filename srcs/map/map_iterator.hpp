#pragma once

#include "../utils/utils.hpp"
#include <iterator>

namespace ft {


template <class Key, class Value>
struct map_node;

template <class Key, class Value, bool is_const>
class map_iterator
{
    public:
        typedef std::pair<const Key, Value> value_type;
        typedef std::ptrdiff_t difference_type;
        typedef typename choose<is_const, const value_type *, value_type *>::type pointer;
        typedef typename choose<is_const, const value_type &, value_type &>::type reference;
        typedef std::bidirectional_iterator_tag iterator_category;

    private:
        typedef Key key_type;
        typedef Value mapped_type;
        typedef map_iterator<key_type, mapped_type, is_const> self_type;
        typedef typename ft::choose<is_const, const map_node<key_type, mapped_type> *, map_node<key_type, mapped_type> *>::type node_pointer;

    public:
        map_iterator (node_pointer node = NULL) : _node(node) {}
        map_iterator (const map_iterator<key_type, value_type, false>& other) : _node(other._node) {}
        map_iterator &operator=(const self_type & other)
        {
            if (this != &other)
                _node = other._node;
            return *this;
        }

        bool operator==(const self_type & other) { return _node == other._node; }
        bool operator!=(const self_type & other) { return _node != other._node; }

        reference operator*(void) { return _node->p; }
        pointer operator->(void) { return reinterpret_cast<value_type*>(&_node->p); }

        /* if there is a right subtree goto it's smallest node
         * else if we are done w/ left subtree go to subtree root
         * else if we are done w/ right subtree go to subtree root parent
         */
        self_type & operator++(void)
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

        self_type operator++(int)
        {
            self_type tmp = *this;
            ++*this;
            return tmp;
        }

        self_type & operator--(void)
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

        self_type operator--(int)
        {
            self_type tmp = *this;
            --*this;
            return tmp;
        }

    private:
        node_pointer _node;

}; // class map_iterator

template <class Key, class Value, bool is_const>
class reverse_map_iterator
{
    public:
        typedef std::pair<const Key, Value> value_type;
        typedef std::ptrdiff_t difference_type;
        typedef typename choose<is_const, const value_type *, value_type *>::type pointer;
        typedef typename choose<is_const, const value_type &, value_type &>::type reference;
        typedef std::bidirectional_iterator_tag iterator_category;

    private:
        typedef Key key_type;
        typedef Value mapped_type;
        typedef reverse_map_iterator<key_type, mapped_type, is_const> self_type;
        typedef typename ft::choose<is_const, const map_node<key_type, mapped_type> *, map_node<key_type, mapped_type> *>::type node_pointer;

    public:
        reverse_map_iterator (node_pointer node = NULL) : _node(node) {}
        reverse_map_iterator (const reverse_map_iterator<key_type, value_type, false>& other) : _node(other._node) {}
        reverse_map_iterator &operator=(const self_type & other)
        {
            if (this != &other)
                _node = other._node;
            return *this;
        }

        bool operator==(const self_type & other) { return _node == other._node; }
        bool operator!=(const self_type & other) { return _node != other._node; }

        reference operator*(void) { return _node->p; }
        pointer operator->(void) { return &(_node->p); }

        /* if there is a right subtree goto it's smallest node
         * else if we are done w/ left subtree go to subtree root
         * else if we are done w/ right subtree go to subtree root parent
         */
        self_type & operator++(void)
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

        self_type operator++(int)
        {
            self_type tmp = *this;
            ++*this;
            return tmp;
        }

        self_type & operator--(void)
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

        self_type operator--(int)
        {
            self_type tmp = *this;
            --*this;
            return tmp;
        }

    private:
        node_pointer _node;

}; // class reverse_map_iterator

} // namespace ft
