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

        typedef map_iterator<key_type, mapped_type, false> iterator;
        typedef map_iterator<key_type, mapped_type, true> const_iterator;
        typedef reverse_map_iterator<key_type, mapped_type, false> reverse_iterator;
        typedef reverse_map_iterator<key_type, mapped_type, true> const_reverse_iterator;
        typedef std::ptrdiff_t difference_type;
        typedef size_t size_type;

    public:
        /* Constructors */
        explicit map (const allocator_type& alloc = allocator_type())
            : _alloc(alloc), _key_comp(key_compare())
        {
        }

        template <class InputIterator>
        map (InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type(),
              typename ft::enable_if< !std::is_integral<InputIterator>::value , void >::type* = 0)
            : _alloc(alloc), _key_comp(key_compare())
        {
            for (; first != last; ++first)
                _tree.insert(*first);
        }

        map (const map & x)
            : _alloc(x._alloc), _key_comp(x._key_comp)
        {
            *this = x;
        }

        /* Destructor */
        ~map()
        {
        }

        /* Operators */
        map &operator=(const map & x)
        {
            if (this != &x) {
                _tree.clear();
                for (iterator it = x.begin(); it != x.end(); ++it)
                    _tree.insert(*it);
            }
        }

        mapped_type& operator[] (const key_type& k);

        friend bool operator== (const map<T, Alloc>& lhs, const map<T, Alloc>& rhs);
        friend bool operator< (const map<T, Alloc>& lhs, const map<T, Alloc>& rhs);
        friend bool operator!= (const map<T, Alloc>& lhs, const map<T, Alloc>& rhs) { return !(lhs == rhs); }
        friend bool operator<= (const map<T, Alloc>& lhs, const map<T, Alloc>& rhs) { return !(rhs < lhs); }
        friend bool operator> (const map<T, Alloc>& lhs, const map<T, Alloc>& rhs) { return rhs < lhs; }
        friend bool operator>= (const map<T, Alloc>& lhs, const map<T, Alloc>& rhs) { return !(lhs < rhs); }

        /* Iterators */
        iterator begin() { return iterator(_tree.begin()); }
        const_iterator begin() const { return const_iterator(_tree.begin()); }
        iterator end() { return iterator(_tree.end()); }
        const_iterator end() const { return const_iterator(_tree.end()); }
        reverse_iterator rbegin() { return reverse_iterator(_tree.rbegin()); }
        const_reverse_iterator rbegin() const { return const_reverse_iterator(_tree.rbegin()); }
        reverse_iterator rend() { return reverse_iterator(_tree.rend()); }
        const_reverse_iterator rend() const { return const_reverse_iterator(_tree.rend()); }

        /* Capacity */
        bool empty() const { return _tree.empty(); }
        size_type size() const { return _tree.size(); }
        size_type max_size() const { return _alloc.max_size(); }

        /* Modifiers */
        std::pair<iterator, bool> insert (const value_type& val) { return _tree.insert(val); }
        iterator insert (iterator position, const value_type& val) { return _tree.insert(position, val)->first; }

        template <class InputIterator>
        void insert (InputIterator first, InputIterator last,
              typename ft::enable_if< !std::is_integral<InputIterator>::value , void >::type* = 0)
        {
            for (; first != last; ++first)
                _tree.insert(*first);
        }

        void erase (iterator position);

        size_type erase (const key_type& k)
        {
            return _tree.erase(k);
        }

        void erase (iterator first, iterator last);
        void clear (void) { _tree.clear(); }
        void swap (map& x); // usage of std::swap in <algorithm> forbidden ?

        /* OPERATIONS */
        size_type count (const key_type& k) const
        {
            if (_tree.find(k))
                return 1;
            return 0;
        }

        iterator find (const key_type& k) { return iterator(_tree.find(k)); }
        const_iterator find (const key_type& k) const { return const_iterator(_tree.find(k)); }

        std::pair<iterator, iterator> equal_range (const key_type& k);
        std::pair<const_iterator, const_iterator> equal_range (const key_type& k) const;
        iterator lower_bound (const key_type& k);
        const_iterator lower_bound (const key_type& k) const;
        iterator upper_bound (const key_type& k);
        const_iterator upper_bound (const key_type& k) const;

        /* DEBUG */
        void print_tree(void)
        {
            _tree.print_tree();
        }


        /* OBSERVERS */
        key_compare key_comp() const { return _key_comp; }
        //value_compare value_comp() const;

    private:
        LLRB<key_type, mapped_type> _tree;
        allocator_type _alloc;
        key_compare _key_comp;


}; // class map

template <class T, class Alloc>
void swap (map<T,Alloc>& x, map<T,Alloc>& y);

} // namespace ft
