#pragma once

#include <iostream>
#include <memory>
#include <limits>
#include <type_traits>

#include "map_iterator.hpp"
#include "../utils/llrb_tree.hpp"

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

        typedef map_iterator<value_type, false> iterator;
        typedef map_iterator<value_type, true> const_iterator;
        typedef reverse_map_iterator<value_type, false> reverse_iterator;
        typedef reverse_map_iterator<value_type, true> const_reverse_iterator;
        typedef std::ptrdiff_t difference_type;
        typedef size_t size_type;

    public:
        /* Constructors */
        explicit map (const allocator_type& alloc = allocator_type())
            : _key_comp(key_compare())
        {
        }

        explicit map (size_t n, const value_type & val = value_type(), const allocator_type& alloc = allocator_type());
        template <class InputIterator>
        map (InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type(),
              typename ft::enable_if< !std::is_integral<InputIterator>::value , void >::type* = 0);
        map (const map & x);

        /* Destructor */
        ~map();

        /* Operators */
        map &operator=(const map & x);
        mapped_type& operator[] (const key_type& k);
        friend bool operator== (const map<T, Alloc>& lhs, const map<T, Alloc>& rhs);
        friend bool operator< (const map<T, Alloc>& lhs, const map<T, Alloc>& rhs);
        friend bool operator!= (const map<T, Alloc>& lhs, const map<T, Alloc>& rhs) { return !(lhs == rhs); }
        friend bool operator<= (const map<T, Alloc>& lhs, const map<T, Alloc>& rhs) { return !(rhs < lhs); }
        friend bool operator> (const map<T, Alloc>& lhs, const map<T, Alloc>& rhs) { return rhs < lhs; }
        friend bool operator>= (const map<T, Alloc>& lhs, const map<T, Alloc>& rhs) { return !(lhs < rhs); }

        /* Iterators */
        //iterator begin() { return iterator(_head->next); }
        //const_iterator begin() const { return const_iterator(_head->next); }
        //iterator end() { return iterator(_tail); }
        //const_iterator end() const { return const_iterator(_tail); }

        //reverse_iterator rbegin() { return reverse_iterator(_tail->prev); }
        //const_reverse_iterator rbegin() const { return const_reverse_iterator(_tail->prev); }
        //reverse_iterator rend() { return reverse_iterator(_head); }
        //const_reverse_iterator rend() const { return const_reverse_iterator(_head); }

        /* Capacity */
        bool empty() const { return _tree.empty(); }
        size_type size() const { return _tree.size(); }
        size_type max_size() const { return _alloc.max_size(); }

        /* Modifiers */
        std::pair<iterator, bool> insert (const value_type& val)
        {
            return _tree.insert(val);
        }

        iterator insert (iterator position, const value_type& val);
        template <class InputIterator>
        void insert (InputIterator first, InputIterator last,
              typename ft::enable_if< !std::is_integral<InputIterator>::value , void >::type* = 0);

        void erase (iterator position);
        size_type erase (const key_type& k);
        void erase (iterator first, iterator last);
        void clear (void);
        void swap (map& x); // usage of std::swap in <algorithm> forbidden ?

        /* Operations */
        size_type count (const key_type& k) const;
        iterator find (const key_type& k);
        const_iterator find (const key_type& k) const;

        std::pair<iterator, iterator> equal_range (const key_type& k);
        std::pair<const_iterator, const_iterator> equal_range (const key_type& k) const;
        iterator lower_bound (const key_type& k);
        const_iterator lower_bound (const key_type& k) const;
        iterator upper_bound (const key_type& k);
        const_iterator upper_bound (const key_type& k) const;

        /* Observers */
        key_compare key_comp() const { return _key_comp; }
        //value_compare value_comp() const;

    private:
        LLRB<value_type> _tree;
        allocator_type _alloc;
        key_compare _key_comp;

}; // class map

template <class T, class Alloc>
void swap (map<T,Alloc>& x, map<T,Alloc>& y);

} // namespace ft
