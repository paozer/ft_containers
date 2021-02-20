#pragma once

#include <memory> // std::allocator
#include <cstddef> // std::ptddiff_t, size_t
#include <limits> // std::numeric_limits
#include <functional> // std::less
#include <utility> // std::pair

#include "map_iterator.hpp"
#include "../utils/avl_tree.hpp"

namespace ft {

template < class Key,
           class T,
           class Compare = std::less<Key>,
           class Alloc = std::allocator<std::pair<const Key, T> >
           >
class map
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
        typedef map_iterator<key_type, mapped_type, false> iterator;
        typedef map_iterator<key_type, mapped_type, true> const_iterator;
        typedef reverse_map_iterator<key_type, mapped_type, false> reverse_iterator;
        typedef reverse_map_iterator<key_type, mapped_type, true> const_reverse_iterator;
        typedef std::ptrdiff_t difference_type;
        typedef size_t size_type;

    private:
        typedef ft::avl_node<key_type, mapped_type> node;
        typedef node * node_pointer;

    public:
        /* CONSTRUCTORS */
        explicit map (const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
            : _tree(comp, alloc), _comp(comp), _alloc(alloc)
        {
        }

        template <class InputIterator>
        map (InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type(),
              typename ft::enable_if< !std::numeric_limits<InputIterator>::is_integer , void >::type* = 0)
            : _tree(comp, alloc), _comp(comp), _alloc(alloc)
        {
            for (; first != last; ++first)
                _tree.insert(*first);
        }

        map (const map & x) { *this = x; }

        /* DESTRUCTOR */
        ~map()
        {
        }

        /* OPERATORS */
        map &operator=(const map & x)
        {
            if (this != &x) {
                //_alloc = x._alloc; // don't do this please
                _comp = x._comp;
                _tree.clear();
                for (const_iterator it = x.begin(); it != x.end(); ++it)
                    _tree.insert(*it);
            }
            return *this;
        }

        mapped_type& operator[] (const key_type& k)
        {
            return _tree.insert(std::make_pair(k, mapped_type())).first->pair.second;
        }

        friend bool operator== (const map<T, Alloc>& lhs, const map<T, Alloc>& rhs);
        friend bool operator< (const map<T, Alloc>& lhs, const map<T, Alloc>& rhs);
        friend bool operator!= (const map<T, Alloc>& lhs, const map<T, Alloc>& rhs) { return !(lhs == rhs); }
        friend bool operator<= (const map<T, Alloc>& lhs, const map<T, Alloc>& rhs) { return !(rhs < lhs); }
        friend bool operator> (const map<T, Alloc>& lhs, const map<T, Alloc>& rhs) { return rhs < lhs; }
        friend bool operator>= (const map<T, Alloc>& lhs, const map<T, Alloc>& rhs) { return !(lhs < rhs); }

        /* ITERATORS */
        // explicit construction not necessary
        iterator begin() { return iterator(_tree.begin()); }
        const_iterator begin() const { return const_iterator(_tree.begin()); }
        iterator end() { return iterator(_tree.end()); }
        const_iterator end() const { return const_iterator(_tree.end()); }
        reverse_iterator rbegin() { return reverse_iterator(_tree.rbegin()); }
        const_reverse_iterator rbegin() const { return const_reverse_iterator(_tree.rbegin()); }
        reverse_iterator rend() { return reverse_iterator(_tree.rend()); }
        const_reverse_iterator rend() const { return const_reverse_iterator(_tree.rend()); }

        /* CAPACITY */
        bool empty() const { return _tree.empty(); }
        size_type size() const { return _tree.size(); }
        size_type max_size() const { return _tree.max_size(); }

        /* MODIFIERS */
        std::pair<iterator, bool> insert (const value_type& val)
        {
            std::pair<node_pointer, bool> ret = _tree.insert(val);
            return std::make_pair(iterator(ret.first), ret.second);
        }

        iterator insert (iterator position, const value_type& val)
        {
            return iterator(_tree.insert(position.get_node(), val).first);
        }

        template <class InputIterator>
        void insert (InputIterator first, InputIterator last,
              typename ft::enable_if< !std::numeric_limits<InputIterator>::is_integer , void >::type* = 0)
        {
            for (; first != last; ++first)
                _tree.insert(*first);
        }

        void erase (iterator position)
        {
            _tree.erase(position.get_node());
        }

        size_type erase (const key_type& k)
        {
            return _tree.erase(k);
        }

        void erase (iterator first, iterator last)
        {
            for (iterator tmp = first; first != last; first = tmp) {
                ++tmp;
                _tree.erase(first.get_node());
            }
        }

        void swap (map& x)
        {
            ft::swap(*this, x);
        }

        void clear(void)
        {
            _tree.clear();
        }

        /* OPERATIONS */
        size_type count (const key_type& k) const
        {
            if (_tree.find(k))
                return 1;
            return 0;
        }

        iterator find (const key_type& k)
        {
            node_pointer node = _tree.find(k);
            if (!node)
                return end();
            return iterator(node);
        }

        const_iterator find (const key_type& k) const
        {
            node_pointer node = _tree.find(k);
            if (!node)
                return end();
            return const_iterator(node);
        }

        std::pair<iterator, iterator> equal_range (const key_type& k);
        std::pair<const_iterator, const_iterator> equal_range (const key_type& k) const;
        iterator lower_bound (const key_type& k);
        const_iterator lower_bound (const key_type& k) const;
        iterator upper_bound (const key_type& k);
        const_iterator upper_bound (const key_type& k) const;

        /* OBSERVERS */
        //key_compare key_comp() const;
        //value_compare value_comp() const;

    private:
        ft::avl_tree<key_type, mapped_type, key_compare, allocator_type> _tree;
        key_compare _comp;
        allocator_type _alloc;


}; // CLASS MAP

template <class T, class Alloc>
void swap (map<T,Alloc>& x, map<T,Alloc>& y);

} // NAMESPACE FT
