#pragma once

#include "../utils/utils.hpp"
#include <iterator>

namespace ft {


template <class T>
struct node_s ;

template <class T, bool is_const>
class map_iterator
{
    public:
        typedef T value_type;
        typedef std::ptrdiff_t difference_type;
        typedef typename choose<is_const, const T *, T *>::type pointer;
        typedef typename choose<is_const, const T &, T &>::type reference;
        typedef std::bidirectional_iterator_tag iterator_category;

    private:
        typedef map_iterator<value_type, is_const> self_type;
        typedef typename ft::choose<is_const, const node_s<T> *, node_s<T> *>::type node_pointer;

    public:

        // use typedef node_pointer
        map_iterator (node_pointer node = NULL) : _node(node) {}

        map_iterator (const map_iterator<value_type, false>& other) : _node(other._node) {}

        map_iterator &operator=(const self_type & other)
        {
            if (this != &other)
                _node = other._node;
            return *this;
        }

        bool operator==(const self_type & other) { return _node == other._node; }
        bool operator!=(const self_type & other) { return _node != other._node; }

        reference operator*(void) { return _node->content; }
        pointer operator->(void) { return &(_node->content); }

        self_type & operator++(void)
        {
            node_pointer parent = _node->prev;
            // handle special case where node is at the root of tree
            if (!parent) {
                // if root has a right subtree goto it's smallest node
                // else we are done
                if (_node->right) {
                    _node = _node->right;
                    while (_node->left)
                        _node = _node->left;
                }
                else
                    _node = NULL;
                return *this;
            }
            else {
                // if right subtree exists go to smallest node of right subtree
                // else if we are in left subtree and done with it goto subtree root
                // else if we are in right subtree and done with it go to subtree root
                if (_node->right) {
                    _node = _node->right;
                    while (_node->left)
                        _node = _node->left;
                }
                else if (_node == parent->left)
                    _node = parent;
                else if (_node == parent->right) {
                    while (_node == parent->right) {
                        _node = parent;
                        parent = _node->prev;
                    }
                    _node = _node->prev;
                }
            }
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

    private:
        node_pointer _node;

}; // class map_iterator

template <class T, bool is_const>
class reverse_map_iterator
{
}; // class reverse_map_iterator

} // namespace ft
