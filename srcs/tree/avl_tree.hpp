#pragma once

#include "avl_node.hpp"
#include "avl_iterator.hpp"
#include "../utils/utils.hpp"

#include <cstddef> // size_t, NULL
#include <cmath> // std::abs

#include <memory> // std::allocator
#include <functional> // std::less
#include <limits> // std::numeric_limits

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
            insert(first, last);
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
        avl_tree& operator= (const avl_tree& x)
        {
            if (this != &x) {
                insert(x.begin(), x.end());
                _alloc = x._alloc;
                _comp = x._comp;
            }
            return *this;
        }

        friend bool operator== (const avl_tree<T, Compare, Alloc>& lhs, const avl_tree<T, Compare, Alloc>& rhs)
        {
            if (lhs._size != rhs._size)
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
            if (lhs._size >= rhs._size)
                return false;
            return true;
        }

        friend bool operator!= (const avl_tree<T, Compare, Alloc>& lhs, const avl_tree<T, Compare, Alloc>& rhs) { return !(lhs == rhs); }
        friend bool operator<= (const avl_tree<T, Compare, Alloc>& lhs, const avl_tree<T, Compare, Alloc>& rhs) { return !(rhs < lhs); }
        friend bool operator> (const avl_tree<T, Compare, Alloc>& lhs, const avl_tree<T, Compare, Alloc>& rhs) { return rhs < lhs; }
        friend bool operator>= (const avl_tree<T, Compare, Alloc>& lhs, const avl_tree<T, Compare, Alloc>& rhs) { return !(lhs < rhs); }

        /* MODIFIERS */
        // TODO: add check if arg is greater than max or lesser than min and use as hint
        std::pair<iterator, bool> insert (const value_type& val)
        {
            unset_bounds();
            _root = aux_insert(NULL, _root, val);
            if (_added_node)
                rebalance(_added_node_ptr);
            set_bounds();
            return std::make_pair(iterator(_added_node_ptr), _added_node);
        }

        iterator insert (iterator position, const value_type& val)
        {
            if (position != end() && _comp(*position, val)) {
                iterator next = position;
                ++next;
                if (next == end() || _comp(val, *next)) {
                    unset_bounds();
                    aux_insert(position.get_node()->parent, position.get_node(), val);
                    // do not need to check if node was inserted since
                    // if prev < val < next there cannot be a duplicate
                    rebalance(_added_node_ptr);
                    set_bounds();
                    return iterator(_added_node_ptr);
                }
            }
            return insert(val).first;
        }

        template <class InputIterator>
        void insert (InputIterator first, InputIterator last,
              typename ft::enable_if< !std::numeric_limits<InputIterator>::is_integer , void >::type* = 0)
        {
            for (; first != last; ++first)
                insert(*first);
        }

        void erase (iterator position)
        {
            --_size;
            unset_bounds();
            node_pointer node = position.get_node();
            node_pointer tmp;
            if (!node->left && !node->right)
                tmp = aux_erase_no_child_node(node);
            else if ((!node->left && node->right) || (node->left && !node->right))
                tmp = aux_erase_one_child_node(node);
            else
                tmp = aux_erase_two_child_node(node);
            recompute_heights(tmp);
            rebalance(tmp);
            set_bounds();
        }

        size_type erase (const value_type& k)
        {
            size_type old_size = _size;
            iterator it = find(k);
            if (it != end())
                erase(it);
            return old_size - _size;
        }

        void erase (iterator first, iterator last)
        {
            for (iterator tmp = first; first != last; first = tmp) {
                ++tmp;
                erase(first);
            }
        }

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
            unset_bounds();
            aux_clear(_root);
            _size = 0;
            _root = NULL;
            set_bounds();
        }

        /* OBSERVERS */
        size_type count (const value_type& val) const
        {
            if (find(val) != end())
                return 1;
            return 0;
        }

        iterator find (const value_type& val)
        {
            node_pointer node = _root;
            while (node && node != _begin && node != _end) {
                if (_comp(node->content, val))
                    node = node->right;
                else if (_comp(val, node->content))
                    node = node->left;
                else
                    return iterator(node);
            }
            return end();
        }

        const_iterator find (const value_type& val) const
        {
            node_pointer node = _root;
            while (node && node != _begin && node != _end) {
                if (_comp(node->content, val))
                    node = node->right;
                else if (_comp(val, node->content))
                    node = node->left;
                else
                    return const_iterator(node);
            }
            return end();
        }

        std::pair<iterator, iterator> equal_range (const value_type& val)
        {
            iterator low = lower_bound(val);
            iterator up = upper_bound(val);
            return std::make_pair(low, up);
        }

        std::pair<const_iterator, const_iterator> equal_range (const value_type& val) const
        {
            const_iterator low = lower_bound(val);
            const_iterator up = upper_bound(val);
            return std::make_pair(low, up);
        }

        // lower_bound returns iterator to first element that is not less than k
        // upper_bound returns iterator to first element that is greater than k
        iterator lower_bound (const value_type& val)
        {
            iterator ite = end();
            for (iterator it = begin(); it != ite; ++it) {
                if (!_comp(*it, val))
                    return it;
            }
            return ite;
        }

        const_iterator lower_bound (const value_type& val) const
        {
            const_iterator ite = end();
            for (const_iterator it = begin(); it != ite; ++it) {
                if (!_comp(*it, val))
                    return it;
            }
            return ite;
        }

        iterator upper_bound (const value_type& val)
        {
            iterator ite = end();
            for (iterator it = begin(); it != ite; ++it) {
                if (_comp(val, *it))
                    return it;
            }
            return ite;
        }

        const_iterator upper_bound (const value_type& val) const
        {
            const_iterator ite = end();
            for (const_iterator it = begin(); it != ite; ++it) {
                if (_comp(val, *it))
                    return it;
            }
            return ite;
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

        /* INSERT */
        node_pointer aux_insert (node_pointer parent, node_pointer node, const value_type& val)
        {
            if (!node) {
                node = new_node(val);
                node->parent = parent;
                _added_node = true;
                _added_node_ptr = node;
                ++_size;
            } else if (_comp(val, node->content)) {
                node->left = aux_insert(node, node->left, val);
            } else if (_comp(node->content, val)) {
                node->right = aux_insert(node, node->right, val);
            } else {
                _added_node = false;
                _added_node_ptr = node;
            }
            return node;
        }

        /* ERASE */
        node_pointer aux_erase_no_child_node (node_pointer child)
        {
            node_pointer parent = child->parent;
            if (parent) {
                if (parent->left == child)
                    parent->left = NULL;
                else
                    parent->right = NULL;
            } else {
                _root = NULL;
            }
            delete_node(child);
            return parent;
        }

        node_pointer aux_erase_one_child_node (node_pointer child)
        {
            node_pointer parent = child->parent;
            if (parent) {
                if (parent->left == child)
                    parent->left = (child->left ? child->left : child->right);
                else if (parent->right == child)
                    parent->right = (child->left ? child->left : child->right);
            } else {
                _root = (child->left ? child->left : child->right);
            }
            if (child->left)
                child->left->parent = parent;
            else
                child->right->parent = parent;
            delete_node(child);
            return parent;
        }

        node_pointer aux_erase_two_child_node (node_pointer child)
        {
            node_pointer min = get_min(child->right);
            swap_nodes(child, min);
            if (!child->left && !child->right)
                return aux_erase_no_child_node(child);
            return aux_erase_one_child_node(child);
        }

        /* REBALANCING */

        // TODO: use bf instead of height and break when
        // deletion/insertion has been balanced out
        void rebalance (node_pointer node)
        {
            for (; node; node = node->parent) {
                int lheight = (node->left) ? node->left->height : -1;
                int rheight = (node->right) ? node->right->height : -1;
                int bf = rheight - lheight;
                if (bf == 2) {
                    if (!node->right->right)
                        node->right = right_rotate(node->right);
                    node = left_rotate(node);
                    break ;
                } else if (bf == -2) {
                    if (!node->left->left)
                        node->left = left_rotate(node->left);
                    node = right_rotate(node);
                    break ;
                } else {
                    update_height(node);
                }
            }
        }

        int get_height (node_pointer node)
        {
            if (!node)
                return 0;
            int left_height = get_height(node->left);
            int right_height = get_height(node->right);
            return ft::max(left_height, right_height) + 1;
        }

        void update_height(node_pointer node)
        {
            int lheight = node->left ? node->left->height : -1;
            int rheight = node->right ? node->right->height : -1;
            node->height = ft::max(lheight, rheight) + 1;
        }

        void recompute_heights(node_pointer node)
        {
            if (!node)
                return ;
            update_height(node);
            recompute_heights(node->parent);
        }

        /* ROTATIONS */
        node_pointer left_rotate (node_pointer node)
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
            update_height(tmp->left);
            update_height(tmp);
            return tmp;
        }

        node_pointer right_rotate (node_pointer node)
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
            update_height(tmp->right);
            update_height(tmp);
            return tmp;
        }

        /* UTILITIES */
        // only works in the context of swapping a node with it's
        // inorder successor during the erasal of a node with
        // two children
        void swap_nodes (node_pointer n1, node_pointer n2)
        {
            ft::swap(n1->height, n2->height);

            // swap left children
            n2->left = n1->left;
            n2->left->parent = n2;
            n1->left = NULL;

            // swap right children
            node_pointer tmp = n2->right;
            if (n1->right != n2) {
                n2->right = n1->right;
                n2->right->parent = n2;
            } else {
                // n1->parent will be fixed later
                n2->right = n1;
            }
            n1->right = tmp;
            n1->right ? n1->right->parent = n1 : 0;

            // swap parents
            if (n1->parent == NULL)
                _root = n2;
            else if (n1->parent->left == n1)
                n1->parent->left = n2;
            else
                n1->parent->right = n2;

            tmp = n2->parent;
            n2->parent = n1->parent;
            if (tmp == n1)
                n1->parent = n2;
            else {
                n1->parent = tmp;
                n1->parent->left = n1;
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
            } else {
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

        node_pointer get_min (node_pointer node)
        {
            while (node->left)
                node = node->left;
            return node;
        }

        node_pointer get_max (node_pointer node)
        {
            while (node->right)
                node = node->right;
            return node;
        }

        /* MEMORY MANAGEMENT */
        node_pointer new_node (const value_type& val = value_type())
        {
            node_pointer p = _alloc.allocate(1);
            // implicitly calls avl_node constructor to create tmp node
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
