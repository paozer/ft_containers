#pragma once

#include "../utils/avl_tree.hpp"
#include "../utils/avl_iterator.hpp"

#include <cstddef> // std::ptddiff_t, size_t

#include <memory> // std::allocator
#include <limits> // std::numeric_limits
#include <functional> // std::less
#include <utility> // std::pair

namespace ft {

template < class T,
           class Compare = std::less<T>,
           class Alloc = std::allocator<T>
           >
class multiset : public avl_tree<T, Compare, Alloc>
{
    public:
        typedef T key_type;
        typedef T value_type;
        typedef Compare key_compare;
        typedef Compare value_compare;
        typedef Alloc allocator_type;
        typedef typename allocator_type::reference reference;
        typedef typename allocator_type::const_reference const_reference;
        typedef typename allocator_type::pointer pointer;
        typedef typename allocator_type::const_pointer const_pointer;
        typedef avl_iterator<value_type, false> iterator;
        typedef avl_iterator<value_type, true> const_iterator;
        typedef reverse_avl_iterator<value_type, false> reverse_iterator;
        typedef reverse_avl_iterator<value_type, true> const_reverse_iterator;
        typedef std::ptrdiff_t difference_type;
        typedef size_t size_type;

    private:
        typedef avl_tree<value_type, key_compare, allocator_type> Base;
        typedef avl_node<value_type> node;
        typedef node* node_pointer;

    public:
        /* CONSTRUCTORS */
        explicit multiset (const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
            : _comp(comp), _alloc(alloc)
        {
        }

        template <class InputIterator>
        multiset (InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type(),
              typename ft::enable_if< !std::numeric_limits<InputIterator>::is_integer , void >::type* = 0)
            : _comp(comp), _alloc(alloc)
        {
            insert(first, last);
        }

        multiset (const multiset& x)
            : _comp(x._comp), _alloc(x._alloc)
        {
            *this = x;
        }

        /* DESTRUCTOR */
        ~multiset()
        {
        }

        /* OPERATORS */
        multiset& operator= (const multiset& x)
        {
            if (this != &x) {
                _comp = x._comp;
                Base::clear();
                insert(x.begin(), x.end());
            }
            return *this;
        }

        /* MODIFIERS */
        iterator insert (const value_type& val)
        {
            Base::unset_bounds();
            Base::_root = aux_insert(NULL, Base::_root, val);
            Base::rebalance(Base::_added_node_ptr);
            Base::set_bounds();
            return iterator(Base::_added_node_ptr);
        }

        iterator insert (iterator position, const value_type& val)
        {
            if (position != Base::end() && _comp(*position, val)) {
                iterator next = position;
                ++next;
                if (next == Base::end() || _comp(val, *next)) {
                    Base::unset_bounds();
                    node_pointer position_ptr = position.get_node();
                    aux_insert(position_ptr->parent, position_ptr, val);
                    Base::rebalance(Base::_added_node_ptr);
                    Base::set_bounds();
                    return iterator(Base::_added_node_ptr);
                }
            }
            return insert(val);
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
            if (position == Base::end())
                return ;
            --Base::_size;
            Base::unset_bounds();
            node_pointer node = position.get_node();
            node_pointer tmp;
            if (!node->left && !node->right)
                tmp = Base::aux_erase_no_child_node(node);
            else if ((!node->left && node->right) || (node->left && !node->right))
                tmp = Base::aux_erase_one_child_node(node);
            else
                tmp = Base::aux_erase_two_child_node(node);
            Base::recompute_heights(tmp);
            Base::rebalance(tmp);
            Base::set_bounds();
        }

        size_type erase (const key_type& k)
        {
            size_type old_size = Base::_size;
            std::pair<iterator, iterator> eq = equal_range(k);
            erase(eq.first, eq.second);
            return old_size - Base::_size;
        }

        void erase (iterator first, iterator last)
        {
            for (iterator tmp = first; first != last; first = tmp) {
                ++tmp;
                erase(first);
            }
        }

        /* OBSERVERS */
        key_compare key_comp (void) const { return _comp; }
        value_compare value_comp (void) const { return value_compare(_comp); }

        /* OPERATIONS */
        size_type count (const key_type& k) const
        {
            std::pair<const_iterator, const_iterator> eq = equal_range(k);
            return std::distance(eq.first, eq.second);
        }

        iterator find (const key_type& k)
        {
            node_pointer node = Base::_root;
            while (node && node != Base::_begin && node != Base::_end) {
                if (_comp(node->content, k))
                    node = node->right;
                else if (_comp(k, node->content))
                    node = node->left;
                else
                    return iterator(node);
            }
            return Base::end();
        }

        const_iterator find (const key_type& k) const
        {
            node_pointer node = Base::_root;
            while (node && node != Base::_begin && node != Base::_end) {
                if (_comp(node->content, k))
                    node = node->right;
                else if (_comp(k, node->content))
                    node = node->left;
                else
                    return const_iterator(node);
            }
            return Base::end();
        }

        std::pair<iterator, iterator> equal_range (const key_type& k)
        {
            iterator low = lower_bound(k);
            iterator up = upper_bound(k);
            return std::make_pair(low, up);
        }

        std::pair<const_iterator, const_iterator> equal_range (const key_type& k) const
        {
            const_iterator low = lower_bound(k);
            const_iterator up = upper_bound(k);
            return std::make_pair(low, up);
        }

        // lower_bound returns iterator to first element that is not less than k
        // upper_bound returns iterator to first element that is greater than k
        iterator lower_bound (const key_type& k)
        {
            iterator ite = Base::end();
            for (iterator it = Base::begin(); it != ite; ++it) {
                if (!_comp(*it, k))
                    return it;
            }
            return ite;
        }

        const_iterator lower_bound (const key_type& k) const
        {
            const_iterator ite = Base::end();
            for (const_iterator it = Base::begin(); it != ite; ++it) {
                if (!_comp(*it, k))
                    return it;
            }
            return ite;
        }

        iterator upper_bound (const key_type& k)
        {
            iterator ite = Base::end();
            for (iterator it = Base::begin(); it != ite; ++it) {
                if (_comp(k, *it))
                    return it;
            }
            return ite;
        }

        const_iterator upper_bound (const key_type& k) const
        {
            const_iterator ite = Base::end();
            for (const_iterator it = Base::begin(); it != ite; ++it) {
                if (_comp(k, *it))
                    return it;
            }
            return ite;
        }

    private:
        key_compare _comp;
        allocator_type _alloc;

        node_pointer aux_insert (node_pointer parent, node_pointer node, const value_type& val)
        {
            if (!node) {
                node = Base::new_node(val);
                node->parent = parent;
                Base::_added_node_ptr = node;
                ++Base::_size;
            } else if (_comp(val, node->content)) {
                node->left = aux_insert(node, node->left, val);
            } else if (_comp(node->content, val)) {
                node->right = aux_insert(node, node->right, val);
            } else {
                node->right = aux_insert(node, node->right, val);
            }
            return node;
        }

}; // CLASS MAP

} // NAMESPACE FT
