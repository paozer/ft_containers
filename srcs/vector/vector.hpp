#pragma once

#include <iostream>
#include <memory>
#include <limits>
#include <type_traits>
#include <stdexcept>
#include <cmath>

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
        typedef reverse_vector_iterator<value_type, false> reverse_iterator;
        typedef reverse_vector_iterator<value_type, true> const_reverse_iterator;
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
            for (size_type i = 0; i < _size; ++i)
                _alloc.destroy(&_element[i]);
            _alloc.deallocate(_element, _capacity);
        }

        /* Iterators */
        iterator begin() { return iterator(0, _size, _element); }
        const_iterator begin() const { return const_iterator(0, _size, _element); }
        iterator end() { return iterator(_size, _size, _element); }
        const_iterator end() const { return const_iterator(_size, _size, _element); }

        reverse_iterator rbegin() { return reverse_iterator(_size, _size, _element); }
        const_reverse_iterator rbegin() const { return const_reverse_iterator(_size, _size, _element); }
        reverse_iterator rend() { return reverse_iterator(0, _size, _element); }
        const_reverse_iterator rend() const { return const_reverse_iterator(0, _size, _element); }

        /* Capacity */
        size_type size() const { return _size; }
        size_type max_size() const { return std::numeric_limits<size_type>::max() / sizeof(value_type) ; }
        //void resize (size_type n, value_type val = value_type());
        size_type capacity() const { return _capacity; }
        bool empty() const { return _size == 0; }

        void reserve (size_type n)
        {
            if (n > _capacity) {
                value_type * new_elem = allocate_big_enough_array(n);
                copy(iterator(0, 0, new_elem), begin(), end());
                for (size_type i = 0; i < _size; ++i)
                    _alloc.destroy(&_element[i]);
                _alloc.deallocate(_element, _capacity);
                _capacity = n;
                _element = new_elem;
            }
        }

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

        void push_back (const value_type& val) { insert(end(), val); }
        void pop_back (void) { erase(--end()); }

        iterator insert (iterator position, const value_type& val)
        {
            insert(position, 1, val);
            return iterator(position.get_index() - 1, _size, _element);
        }

        void insert (iterator position, size_type n, const value_type& val)
        {
            if (_size + n > _capacity) {
                // alloc new space (multiple of 1.5 capacity)
                _capacity == 0 ? _capacity = 1 : 0;
                while (_size + n > _capacity)
                    _capacity = ceil(_capacity * 1.5);
                value_type * new_element = _alloc.allocate(_capacity);
                // copy old values until position
                iterator it = begin();
                size_type i = 0;
                while (it != position) {
                    new_element[i] = *it;
                    ++it;
                    ++i;
                }
                // insert n val
                for (size_type tmp = i; i - tmp < n; ++i)
                    new_element[i] = val;
                // copy old values after position
                for (; it != end(); ++it)
                    new_element[i] = *it;
                // destroy and deallocate
                for (size_type i = 0; i < _size; ++i)
                    _alloc.destroy(&(_element[i]));
                _alloc.deallocate(_element, _capacity);
                // update _element, _size
                _element = new_element;
            }
            else {
                // right shift values by n begining at position
                for (size_type i = 0; i < _size - position.get_index(); ++i) {
                    _element[i + _size] = _element[i + position.get_index()];
                }
                // insert n val before position
                for (size_type i = 0; i < n; ++i) {
                    _element[i + position.get_index()] = val;
                }
            }
            _size += n;
        }

        // allocate new_array (new_size) returns ptr
        // copy from iterator to position into array @ index copy(array, index, first, last)
        // destroy & deallocate array

        template <class InputIterator>
        void insert (iterator position, InputIterator first, InputIterator last,
              typename ft::enable_if< !std::is_integral<InputIterator>::value , void >::type* = 0)
        {
            size_type new_size = _size + std::distance(first, last);
            if (new_size > _capacity) {

                value_type * new_element = allocate_big_enough_array(new_size);
                // copy old values until position
                iterator it = begin();
                size_type i = 0;
                while (it != position) {
                    new_element[i] = *it;
                    ++it;
                    ++i;
                }
                // insert from first to last
                for (; first != last; ++first) {
                    new_element[i] = *first;
                    ++i;
                }
                // copy old values after position
                for (; it != end(); ++it)
                    new_element[i] = *it;
                // destroy and deallocate
                for (size_type i = 0; i < _size; ++i)
                    _alloc.destroy(&(_element[i]));
                _alloc.deallocate(_element, _capacity);
                // update _element, _size
                _element = new_element;
            }
            else {
                // right shift values by n begining at position
                for (size_type i = 0; i < _size - position.get_index(); ++i) {
                    _element[i + _size] = _element[i + position.get_index()];
                }
                // insert first to last before position
                for (size_type i = 0; first != last; ++first) {
                    _element[i + position.get_index()] = *first;
                    ++i;
                }
            }
            _size = new_size;
        }


        iterator erase (iterator position)
        {
            iterator next = position;
            ++next;
            return erase(position, next);
        }

        iterator erase (iterator first, iterator last)
        {
            iterator tmp = last;
            while (first != last) {
                _alloc.destroy(&(*first));
                if (tmp != end()) {
                    *first = *tmp;
                    ++tmp;
                }
                ++first;
                --_size;
            }
        }

        void swap (vector& x)
        {
            {   // size and capacity swap
                size_type tmp;
                tmp = _size;
                _size = x._size;
                x._size = tmp;
                tmp = _capacity;
                _capacity = x._capacity;
                x._capacity = tmp;
            }
            {   // element swap
                value_type * tmp = _element;
                _element = x._element;
                x._element = tmp;
            }
            {   // allocator swap
                allocator_type tmp = _alloc;
                _alloc = x._alloc;
                x._alloc = x._alloc;
            }
        }

        void clear()
        {
            for (size_type i = 0; i < _size; ++i)
                _alloc.destroy(&_element[i]);
            _size = 0;
        }

    private:
        size_type _size;
        size_type _capacity;
        value_type * _element;
        Alloc _alloc;

        // does more than one thing: updates this->_capacity
        value_type * allocate_big_enough_array(size_type size)
        {
            _capacity == 0 ? _capacity = 1 : 0;
            while (size > _capacity)
                _capacity = ceil(_capacity * 1.5);
            return _alloc.allocate(_capacity);
        }

        void copy(iterator into, iterator first, iterator last)
        {
            while (first != last) {
                *into = *first;
                ++into;
                ++first;
            }
        }

}; // class vector

template <class T, class Alloc>
void swap (vector<T,Alloc>& x, vector<T,Alloc>& y) { x.swap(y); }

} // namespace vector
