#pragma once

#include "avl_node.hpp"
#include "avl_iterator.hpp"
#include "utils.hpp"

#include <cstddef> // size_t, NULL
#include <cmath> // std::abs

#include <memory> // std::allocator
#include <functional> // std::less

namespace ft {

template <class T,
          class Compare = std::less<T>,
          class Alloc = std::allocator<T>
          >
class avl_tree
{
    public:
        typedef T value_type;
        typedef avl_iterator<T, false> iterator;
        typedef avl_iterator<T, true> const_iterator;
        typedef reverse_avl_iterator<T, false> reverse_iterator;
        typedef reverse_avl_iterator<T, true> const_reverse_iterator;
        typedef Compare compare;
        typedef Alloc allocator_type;
        typedef std::ptrdiff_t difference_type;
        typedef size_t size_type;

    private:
        typedef avl_node<value_type> node;
        typedef node * node_pointer;
        typedef typename Alloc::template rebind<node>::other node_allocator;

    public:
        /* CONSTRUCTORS */
        explicit avl_tree (const compare& comp = compare(), const allocator_type& alloc = allocator_type())
            : _root(NULL), _begin(new_node()), _end(new_node()), _added_node_ptr(NULL),
              _added_node(false), _size(0), _alloc(alloc), _comp(comp)
        {
            _begin->parent = _end;
            _end->parent = _begin;
        }

        template <class InputIterator>
        avl_tree (InputIterator first, InputIterator last, const compare& comp = compare(), const allocator_type& alloc = allocator_type(),
              typename ft::enable_if< !std::numeric_limits<InputIterator>::is_integer , void >::type* = 0)
            : _root(NULL), _begin(new_node()), _end(new_node()), _added_node_ptr(NULL),
              _added_node(false), _size(0), _alloc(alloc), _comp(comp)
        {
            for (; first != last; ++first)
                insert(*first);
        }

        avl_tree (const avl_tree& x)
        {
            *this = x;
        }

        /* DESTRUCTOR */
        ~avl_tree()
        {
            clear();
            delete_node(_begin);
            delete_node(_end);
        }

        /* OPERATORS */
        friend bool operator== (const avl_tree<T, Compare, Alloc>& lhs, const avl_tree<T, Compare, Alloc>& rhs)
        {
            if (lhs.size() != rhs.size())
                return false;
            const_iterator lit = lhs.begin();
            const_iterator rit = rhs.begin();
            while (lit != lhs.end()) {
                if (*lit != *rit)
                    return false;
                ++lit;
                ++rit;
            }
            return true;
        }

        friend bool operator< (const avl_tree<T, Compare, Alloc>& lhs, const avl_tree<T, Compare, Alloc>& rhs)
        {
            const_iterator lit = lhs.begin();
            const_iterator rit = rhs.begin();
            while (lit != lhs.end() && rit != rhs.end()) {
                if (*lit < *rit)
                    return true;
                if (*lit > *rit)
                    return false;
                ++lit;
                ++rit;
            }
            if (lhs.size() >= rhs.size())
                return false;
            return true;
        }

        friend bool operator!= (const avl_tree<T, Compare, Alloc>& lhs, const avl_tree<T, Compare, Alloc>& rhs) { return !(lhs == rhs); }
        friend bool operator<= (const avl_tree<T, Compare, Alloc>& lhs, const avl_tree<T, Compare, Alloc>& rhs) { return !(rhs < lhs); }
        friend bool operator> (const avl_tree<T, Compare, Alloc>& lhs, const avl_tree<T, Compare, Alloc>& rhs) { return rhs < lhs; }
        friend bool operator>= (const avl_tree<T, Compare, Alloc>& lhs, const avl_tree<T, Compare, Alloc>& rhs) { return !(lhs < rhs); }

        /* ELEMENT ACCESS */
        iterator begin() { return iterator(_begin->parent); }
        const_iterator begin() const { return const_iterator(_begin->parent); }
        iterator end() { return iterator(_end); }
        const_iterator end() const { return const_iterator(_end); }
        reverse_iterator rbegin() { return reverse_iterator(_end->parent); }
        const_reverse_iterator rbegin() const { return const_reverse_iterator(_end->parent); }
        reverse_iterator rend() { return reverse_iterator(_begin); }
        const_reverse_iterator rend() const { return const_reverse_iterator(_begin); }

        /* CAPACITY */
        size_type size (void) const { return _size; }
        bool empty (void) const { return _size == 0; }
        size_type max_size (void) const { return _alloc.max_size(); }

        /* MODIFIERS */
        void swap (avl_tree& x)
        {
            ft::swap(_root, x._root);
            ft::swap(_begin, x._begin);
            ft::swap(_end, x._end);
            ft::swap(_size, x._size);
            ft::swap(_alloc, x._alloc);
            ft::swap(_added_node_ptr, x._added_node_ptr);
            ft::swap(_added_node, x._added_node);
        }

        void clear (void)
        {
            if (_size == 0)
                return ;
            unset_bounds();
            aux_clear(_root);
            _size = 0;
            _root = NULL;
            set_bounds();
        }

    protected:
        node_pointer _root;
        node_pointer _begin;
        node_pointer _end;
        node_pointer _added_node_ptr;
        bool _added_node;
        size_type _size;
        node_allocator _alloc;
        compare _comp;

        /* ERASE */
        void aux_erase_no_child_node (node_pointer child)
        {
            node_pointer parent = child->parent;
            if (parent) {
                if (parent->left == child)
                    parent->left = NULL;
                else if (parent->right == child)
                    parent->right = NULL;
            }
            else
                _root = NULL;
            delete_node(child);
        }

        void aux_erase_one_child_node (node_pointer child)
        {
            node_pointer parent = child->parent;
            if (parent) {
                if (parent->left == child)
                    parent->left = (child->left ? child->left : child->right);
                else if (parent->right == child)
                    parent->right = (child->left ? child->left : child->right);
            }
            else
                _root = (child->left ? child->left : child->right);
            if (child->left)
                child->left->parent = parent;
            else
                child->right->parent = parent;
            delete_node(child);
        }

        void aux_erase_two_child_node (node_pointer child)
        {
            node_pointer min = get_min(child->right);
            swap_nodes(child, min);
            if (!child->left && !child->right)
                this->aux_erase_no_child_node(child);
            else if ((!child->left && child->right) || (child->left && !child->right))
                this->aux_erase_one_child_node(child);
        }

        /* REBALANCING */
        void fix_after_insert (node_pointer child)
        {
            if (!child)
                return ;
            for (node_pointer parent = child->parent; parent; parent = parent->parent) {
                if (parent->right == child) {
                    if (parent->bf == 1) {
                        if (child->bf == -1)
                            parent = right_left_rotate(parent);
                        else
                            parent = left_rotate(parent);
                    }
                    else if (parent->bf == -1)
                        parent->bf = 0;
                    else {
                        parent->bf = 1;
                        child = parent;
                        continue ;
                    }
                }
                else {
                    if (parent->bf == -1) {
                        if (child->bf == 1)
                            parent = left_right_rotate(parent);
                        else
                            parent = right_rotate(parent);
                    }
                    else if (parent->bf == 1)
                        parent->bf = 0;
                    else {
                        parent->bf = -1;
                        child = parent;
                        continue ;
                    }
                }
                break ;
            }
        }

        /* ROTATIONS */
        node_pointer left_rotate (node_pointer node, bool preserve_bfs = false)
        {
            node_pointer tmp = node->right;
            node->right = tmp->left;
            node->right ? node->right->parent = node : 0;
            tmp->left = node;
            tmp->parent = node->parent;
            if (!node->parent)
                _root = tmp;
            else if (node->parent->left == node)
                node->parent->left = tmp;
            else
                node->parent->right = tmp;
            node->parent = tmp;
            if (!preserve_bfs) {
                node->bf = 0;
                tmp->bf = 0;
            }
            return tmp;
        }

        node_pointer right_rotate (node_pointer node, bool preserve_bfs = false)
        {
            node_pointer tmp = node->left;
            node->left = tmp->right;
            node->left ? node->left->parent = node : 0;
            tmp->right = node;
            tmp->parent = node->parent;
            if (!node->parent)
                _root = tmp;
            else if (node->parent->left == node)
                node->parent->left = tmp;
            else
                node->parent->right = tmp;
            node->parent = tmp;
            if (!preserve_bfs) {
                node->bf = 0;
                tmp->bf = 0;
            }
            return tmp;
        }

        node_pointer right_left_rotate (node_pointer node)
        {
            node->right = right_rotate(node->right, true);
            node = left_rotate(node, true);
            node->left->bf = 0;
            node->right->bf = 0;
            if (node->bf > 0)
                node->left->bf = -1;
            else if (node->bf < 0)
                node->right->bf = 1;
            node->bf = 0;
            return node;
        }

        node_pointer left_right_rotate (node_pointer node)
        {
            node->left = left_rotate(node->left, true);
            node = right_rotate(node, true);
            node->left->bf = 0;
            node->right->bf = 0;
            if (node->bf > 0)
                node->left->bf = 1;
            else if (node->bf < 0)
                node->right->bf = -1;
            node->bf = 0;
            return node;
        }

        /* UTILITIES */
        void swap_nodes (node_pointer n1, node_pointer n2)
        {
            node_pointer parent_n1 = n1->parent;
            node_pointer left_n1 = n1->left;
            node_pointer right_n1 = n1->right;
            node_pointer parent_n2 = n2->parent;
            node_pointer left_n2 = n2->left;
            node_pointer right_n2 = n2->right;

            if (parent_n1) {
                if (parent_n1->left == n1)
                    parent_n1->left = n2;
                else
                    parent_n1->right = n2;
                n2->parent = parent_n1;
            }
            else {
                _root = n2;
                n2->parent = NULL;
            }

            n2->left = left_n1;
            left_n1->parent = n2;
            n1->left = left_n2;
            n1->right = right_n2;

            if (right_n2)
                right_n2->parent = n1;
            if (parent_n2 != n1) {
                n2->right = right_n1;
                right_n1->parent = n2;
                parent_n2->left = n1;
                n1->parent = parent_n2;
            }
            else {
                n2->right = n1;
                n1->parent = n2;
            }
        }

        void unset_bounds (void)
        {
            if (_begin->parent && _begin->parent != _end)
                _begin->parent->left = NULL;
            if (_end->parent && _end->parent != _begin)
                _end->parent->right = NULL;
        }

        void set_bounds (void)
        {
            if (_size != 0) {
                node_pointer min = get_min(_root);
                node_pointer max = get_max(_root);
                _begin->parent = min;
                min->left = _begin;
                _end->parent = max;
                max->right = _end;
            }
            else {
                _begin->parent = _end;
                _end->parent = _begin;
            }
        }

        void aux_clear (node_pointer node)
        {
            if (!node)
                return ;
            aux_clear(node->left);
            aux_clear(node->right);
            delete_node(node);
        }

        // add begin/end check
        node_pointer get_min (node_pointer node)
        {
            if (!node)
                return NULL;
            while (node->left)
                node = node->left;
            return node;
        }

        // add begin/end check
        node_pointer get_max (node_pointer node)
        {
            if (!node)
                return NULL;
            while (node->right)
                node = node->right;
            return node;
        }

        /* MEMORY MANAGEMENT */
        node_pointer new_node (const value_type& val = value_type())
        {
            node_pointer p = _alloc.allocate(1);
            _alloc.construct(p, val);
            return p;
        }

        void delete_node (node_pointer p)
        {
            _alloc.destroy(p);
            _alloc.deallocate(p, 1);
        }

}; // CLASS AVL_TREE

template <class T, class Compare, class Alloc>
void swap (avl_tree<T, Compare, Alloc>& x, avl_tree<T, Compare, Alloc>& y)
{
    x.swap(y);
}

} // NAMESPACE FT
