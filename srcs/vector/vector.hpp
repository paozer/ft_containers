#pragma once

#include <iostream>
#include <memory>
#include <limits>
#include <type_traits>
#include <stdexcept>

#include "vector_iterator.hpp"
#include "../utils.hpp"

namespace ft {

template < class T, class Alloc = std::allocator<T> >
class vector
{

    public:
        typedef T value_type;
        typedef Alloc allocator_type;
        typedef typename allocator_type::reference reference;
        typedef typename allocator_type::const_reference const_reference;
        typedef typename allocator_type::pointer pointer;
        typedef typename allocator_type::const_pointer const_pointer;

        typedef vector_iterator<value_type, false> iterator;
        typedef vector_iterator<value_type, true> const_iterator;
        //typedef reverse_vector_iterator<value_type, false> reverse_iterator;
        //typedef reverse_vector_iterator<value_type, true> const_reverse_iterator;
        typedef std::ptrdiff_t difference_type;
        typedef size_t size_type;

    private:
        //typedef typename Alloc::template rebind<node>::other node_allocator;

    public:
        /* Constructors */
        explicit vector (const allocator_type& alloc = allocator_type())
            : _size(0), _capacity(0), _element(NULL), _alloc(Alloc())
        {
        }

        explicit vector (size_type n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type())
            : _size(0), _capacity(0), _element(NULL), _alloc(Alloc())
        {
            assign(n, val);
        }

        template <class InputIterator>
        vector (InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type(),
              typename ft::enable_if< !std::is_integral<InputIterator>::value , void >::type* = 0)
            : _size(0), _capacity(0), _element(NULL), _alloc(Alloc())
        {
            assign(first, last);
        }

        vector (const vector & x)
            : _size(0), _capacity(0), _element(NULL), _alloc(x._alloc)
        {
            *this = x;
        }

        /* Operators */
        vector &operator=(const vector & x)
        {
            if (this != &x)
                assign(x.begin(), x.end());
            return *this;
        }

        friend bool operator== (const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
        {
            if (lhs._size != rhs._size)
                return false;
            typedef vector<T, Alloc>::const_iterator iterator;
            iterator lit = lhs.begin();
            iterator rit = rhs.begin();
            while (lit != lhs.end() && rit != rhs.end()) {
                if (*lit != *rit)
                    return false;
                ++lit;
                ++rit;
            }
            return true;
        }

        friend bool operator< (const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
        {
            typedef vector<T, Alloc>::const_iterator iterator;
            iterator lit = lhs.begin();
            iterator rit = rhs.begin();
            while (lit != lhs.end() && rit != rhs.end()) {
                if (*lit < *rit)
                    return true;
                else if (*lit > *rit)
                    return false;
                ++lit;
                ++rit;
            }
            if (lhs._size >= rhs._size)
                return false;
            return true;
        }

        friend bool operator!= (const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) { return !(lhs == rhs); }
        friend bool operator<= (const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) { return !(rhs < lhs); }
        friend bool operator> (const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) { return rhs < lhs; }
        friend bool operator>= (const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) { return !(lhs < rhs); }

        /* Destructor */
        ~vector()
        {
        }

        /* Iterators */
        iterator begin() { return iterator(0, _size, _element); }
        const_iterator begin() const { return const_iterator(0, _size, _element); }
        iterator end() { return iterator(_size, _size, _element); }
        const_iterator end() const { return const_iterator(_size, _size, _element); }
        //reverse_iterator rbegin() { return reverse_iterator(_tail->prev); }
        //const_reverse_iterator rbegin() const { return const_reverse_iterator(_tail->prev); }
        //reverse_iterator rend() { return reverse_iterator(_head); }
        //const_reverse_iterator rend() const { return const_reverse_iterator(_head); }

        /* Capacity */
        size_type size() const { return _size; }
        size_type max_size() const { return std::numeric_limits<size_type>::max() / sizeof(value_type) ; }
        void resize (size_type n, value_type val = value_type());
        size_type capacity() const { return _capacity; }
        bool empty() const { return _size == 0; }
        //void reserve (size_type n);

        /* Element Access */
        reference front () { return _element[0]; }
        const_reference front () const { return _element[0]; }
        reference back () { return _element[_size - 1]; }
        const_reference back () const { return _element[_size - 1]; }

        reference at (size_type n)
        {
            if (n > _size - 1)
                throw std::out_of_range("ft::vector");
            return _element[n];
        }
        const_reference at (size_type n) const
        {
            if (n > _size - 1)
                throw std::out_of_range("ft::vector");
            return _element[n];
        }

        reference operator[] (size_type n) { return _element[n]; }
        const_reference operator[] (size_type n) const { return _element[n]; }

        /* Modifiers */
        template <class InputIterator>
        void assign (InputIterator first, InputIterator last,
              typename ft::enable_if< !std::is_integral<InputIterator>::value , void >::type* = 0)
        {
            clear();
            insert(begin(), first, last);
        }
        void assign(size_type n, const value_type& val)
        {
            clear();
            insert(begin(), n, val);
        }

        void push_back (const value_type& val);
        void pop_back (void);

        iterator insert (iterator position, const value_type& val);
        void insert (iterator position, size_type n, const value_type& val);
        template <class InputIterator>
        void insert (iterator position, InputIterator first, InputIterator last);

        //iterator erase (iterator position);
        //iterator erase (iterator first, iterator last);

        //void swap (vector& x);
        void clear();

    private:
        size_type _size;
        size_type _capacity;
        value_type * _element;
        Alloc _alloc;

        value_type * allocate_array(size_type n)
        {
            value_type * p = _alloc.allocate(n);
            return p;
        }

        void deallocate_array(value_type * p, size_type n)
        {
            for (size_type i = 0; i < n; ++i)
                _alloc.destroy(p[i]);
            _alloc.deallocate(p);
        }

        void realloc_and_copy_if_needed(size_type n)
        {
            if (_size + n > _capacity) {
                value_type * tmp = _element;
                _capacity *= 1.5;
                _element = allocate_array(_capacity);
                for (size_type i = 0; i < _size; ++i)
                    _element[i] = tmp[i];
                deallocate_array(tmp, _size);
            }
        }

}; // class vector

//template <class T, class Alloc>
//void swap (vector<T,Alloc>& x, vector<T,Alloc>& y);

} // namespace vector
