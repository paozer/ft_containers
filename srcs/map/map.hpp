#pragma once

#include "../utils/avl_tree.hpp"
#include "../utils/avl_iterator.hpp"

#include <cstddef> // std::ptddiff_t, size_t

#include <memory> // std::allocator
#include <limits> // std::numeric_limits
#include <functional> // std::less
#include <utility> // std::pair

namespace ft {

template < class Key,
           class T,
           class Compare = std::less<Key>,
           class Alloc = std::allocator<std::pair<const Key, T> >
           >
class map : public avl_tree<std::pair<const Key, T>, Compare, Alloc>
{
    public:
        typedef Key key_type;
        typedef T mapped_type;
        typedef std::pair<const key_type, mapped_type> value_type;
        typedef Compare key_compare;
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

        class value_compare : std::binary_function<value_type, value_type, bool>
        {   // in C++98, it is required to inherit binary_function<value_type,value_type,bool>
            //friend class map;
            protected:
                key_compare comp;
                value_compare (Compare c) : comp(c) {}  // constructed with map's comparison object
            public:
                typedef bool result_type;
                typedef value_type first_argument_type;
                typedef value_type second_argument_type;
                bool operator() (const value_type& x, const value_type& y) const
                {
                  return comp(x.first, y.first);
                }
        };

    private:
        typedef avl_tree<value_type, key_compare, allocator_type> Base;
        typedef avl_node<value_type> node;
        typedef node* node_pointer;

    public:
        /* CONSTRUCTORS */
        explicit map (const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
            : _comp(comp), _alloc(alloc)
        {
        }

        template <class InputIterator>
        map (InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type(),
              typename ft::enable_if< !std::numeric_limits<InputIterator>::is_integer , void >::type* = 0)
            : _comp(comp), _alloc(alloc)
        {
            for (; first != last; ++first)
                insert(*first);
        }

        map (const map& x)
            : _comp(x._comp), _alloc(x._alloc)
        {
            *this = x;
        }

        /* DESTRUCTOR */
        ~map()
        {
        }

        /* OPERATORS */
        map& operator= (const map& x)
        {
            if (this != &x) {
                _comp = x._comp;
                Base::clear();
                for (const_iterator it = x.begin(); it != x.end(); ++it)
                    insert(*it);
            }
            return *this;
        }

        mapped_type& operator[] (const key_type& k)
        {
            return insert(std::make_pair(k, mapped_type())).first->second;
        }

        /* MODIFIERS */
        std::pair<iterator, bool> insert (const value_type& val)
        {
            if (Base::_root) {
                Base::unset_bounds();
                Base::_root = aux_insert(Base::_root->parent, Base::_root, val);
                if (Base::_added_node)
                    Base::fix_after_insert(Base::_added_node_ptr);
            }
            else
                Base::_root = aux_insert(NULL, NULL, val);
            Base::set_bounds();
            return std::make_pair(iterator(Base::_added_node_ptr), Base::_added_node);
        }

        iterator insert (iterator position, const value_type& val)
        {
            if (position != Base::end() && _comp(position->first, val.first)) {
                iterator next = position;
                ++next;
                if (next == Base::end() || _comp(val.first, next->first)) {
                    Base::unset_bounds();
                    node_pointer position_ptr = position.get_node();
                    position_ptr = aux_insert(position_ptr->parent, position_ptr, val);
                    if (Base::_added_node)
                        Base::fix_after_insert(Base::_added_node_ptr);
                    Base::set_bounds();
                    return iterator(Base::_added_node_ptr);
                }
            }
            return iterator(insert(val).first);
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
            Base::unset_bounds();
            node_pointer node = position.get_node();
            if (!node->left && !node->right)
                Base::aux_erase_no_child_node(node);
            else if ((!node->left && node->right) || (node->left && !node->right))
                Base::aux_erase_one_child_node(node);
            else
                Base::aux_erase_two_child_node(node);
            --Base::_size;
            Base::set_bounds();
        }

        size_type erase (const key_type& k)
        {
            size_type old_size = Base::_size;
            erase(find(k));
            return  old_size - Base::_size;
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
            if (find(k) != Base::end())
                return 1;
            return 0;
        }

        iterator find (const key_type& k)
        {
            node_pointer node = Base::_root;
            while (node && node != Base::_begin && node != Base::_end) {
                if (_comp(node->content.first, k))
                    node = node->right;
                else if (_comp(k, node->content.first))
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
                if (_comp(node->content.first, k))
                    node = node->right;
                else if (_comp(k, node->content.first))
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
                if (!_comp(it->first, k))
                    return it;
            }
            return ite;
        }

        const_iterator lower_bound (const key_type& k) const
        {
            const_iterator ite = Base::end();
            for (const_iterator it = Base::begin(); it != ite; ++it) {
                if (!_comp(it->first, k))
                    return it;
            }
            return ite;
        }

        iterator upper_bound (const key_type& k)
        {
            iterator ite = Base::end();
            for (iterator it = Base::begin(); it != ite; ++it) {
                if (_comp(k, it->first))
                    return it;
            }
            return ite;
        }

        const_iterator upper_bound (const key_type& k) const
        {
            const_iterator ite = Base::end();
            for (const_iterator it = Base::begin(); it != ite; ++it) {
                if (_comp(k, it->first))
                    return it;
            }
            return ite;
        }

    private:
        key_compare _comp;
        allocator_type _alloc;

        node_pointer aux_insert (node_pointer parent, node_pointer node, const value_type& val)
        {
            // if we're at a leaf insert key/val pair
            // else if key goes into the left subtree
            // else if key goes into the right subtree
            // else key is already in the tree
            // remember the added/found node to create iterator after return from recursion
            if (!node) {
                node = Base::new_node(val);
                node->parent = parent;
                Base::_added_node = true;
                Base::_added_node_ptr = node;
                ++Base::_size;
            }
            else if (_comp(val.first, node->content.first))
                node->left = aux_insert(node, node->left, val);
            else if (_comp(node->content.first, val.first))
                node->right = aux_insert(node, node->right, val);
            else {
                Base::_added_node = false;
                Base::_added_node_ptr = node;
            }
            return node;
        }

}; // CLASS MAP

} // NAMESPACE FT
