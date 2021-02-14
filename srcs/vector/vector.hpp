#pragma once

#include <iostream>
#include <memory>
#include <limits>
#include <type_traits>
#include <stdexcept>
#include <cmath>

#include "vector_iterator.hpp"
#include "../utils/utils.hpp"

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

    public:
        /* Constructors */
        explicit vector (const allocator_type& alloc = allocator_type())
            : _size(0), _capacity(0), _element(NULL), _alloc(alloc)
        {
        }

        explicit vector (size_type n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type())
            : _size(0), _capacity(0), _element(NULL), _alloc(alloc)
        {
            push_back_n(n, val);
        }

        template <class InputIterator>
        vector (InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type(),
              typename ft::enable_if< !std::is_integral<InputIterator>::value , void >::type* = 0)
            : _size(0), _capacity(0), _element(NULL), _alloc(alloc)
        {
            for (; first != last; ++first)
                push_back(*first);
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
            for (size_type i = 0; i < lhs._size; ++i) {
                if (lhs[i] != rhs[i])
                    return false;
            }
            return true;
        }

        friend bool operator< (const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
        {
            for (size_type i = 0; i < lhs._size && i < rhs._size; ++i) {
                if (lhs[i] < rhs[i])
                    return true;
                else if (lhs[i] > rhs[i])
                    return false;
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
                _alloc.destroy(_element + i);
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
        size_type max_size() const { return _alloc.max_size(); }

        void resize (size_type n, value_type val = value_type())
        {
            if (n > _size)
                push_back_n(n - _size, val);
            else {
                while (n < _size)
                    pop_back();
            }
        }

        size_type capacity() const { return _capacity; }
        bool empty() const { return _size == 0; }

        void reserve (size_type n)
        {
            if (n > _capacity) {
                value_type * new_element = realloc(n);
                _alloc.deallocate(_element, _size);
                _element = new_element;
            }
        }

        /* Element Access */
        reference front () { return _element[0]; }
        const_reference front () const { return _element[0]; }
        reference back () { return _element[_size - 1]; }
        const_reference back () const { return _element[_size - 1]; }

        reference at (size_type n)
        {
            if (n >= _size)
                throw std::out_of_range("ft::vector");
            return _element[n];
        }

        const_reference at (size_type n) const
        {
            if (n >= _size)
                throw std::out_of_range("ft::vector");
            return _element[n];
        }

        reference operator[] (size_type n) { return _element[n]; }
        const_reference operator[] (size_type n) const { return _element[n]; }

        /* Modifiers */

        // use insert(begin(), first, last) ?
        template <class InputIterator>
        void assign (InputIterator first, InputIterator last,
              typename ft::enable_if< !std::is_integral<InputIterator>::value , void >::type* = 0)
        {
            clear();
            for (; first != last; ++first)
                push_back(*first);
        }

        // use insert(begin(), n, val) ?
        void assign(size_type n, const value_type& val)
        {
            clear();
            push_back_n(n, val);
        }

        void push_back (const value_type& val)
        {
            push_back_n(1, val);
        }

        void pop_back (void)
        {
            if (_size > 0) {
                --_size;
                _alloc.destroy(_element + _size);
            }
        }

        iterator insert (iterator position, const value_type& val)
        {
            insert(position, 1, val);
            return iterator(position.get_index() - 1, _size, _element);
        }

        void insert (iterator position, size_type n, const value_type& val)
        {
            if (position == end()) {
                push_back_n(n, val);
            }
            else {
                size_type new_size = _size + n;
                bool needs_realloc = new_size > _capacity;
                value_type * tmp = _element;

                if (needs_realloc)
                    tmp = realloc(new_size);

                for (size_type i = _size - 1; position.get_index() + n < i; --i) {
                    _alloc.construct(tmp + n + i, _element[i]);
                    _alloc.destroy(_element + i);
                }

                for (size_type i = 0; i < n; ++i)
                    _alloc.construct(tmp + position.get_index() + i, val);

                if (needs_realloc) {
                    _alloc.deallocate(_element, _size);
                    _element = tmp;
                }
                _size = new_size;
            }
        }

        template <class InputIterator>
        void insert (iterator position, InputIterator first, InputIterator last,
              typename ft::enable_if< !std::is_integral<InputIterator>::value , void >::type* = 0)
        {
            if (position == end()) {
                for (; first != last; ++first)
                    push_back(*first);
            }
            else {
                size_type n = std::distance(first, last);
                size_type new_size = _size + n;
                bool needs_realloc = new_size > _capacity;
                value_type * tmp = _element;

                if (needs_realloc)
                    tmp = realloc(new_size);

                size_type i = 0;
                for (iterator it = --end(); position + n < it; --it) {
                    _alloc.construct(tmp + n + i, *it);
                    _alloc.destroy(_element + it.get_index());
                    ++i;
                }

                for (size_type i = 0; first != last; ++i) {
                    _alloc.construct(tmp + position.get_index() + i, *first);
                    ++first;
                }

                if (needs_realloc) {
                    _alloc.deallocate(_element, _size);
                    _element = tmp;
                }
                _size = new_size;
            }
        }


        iterator erase (iterator position)
        {
            iterator next = position;
            ++next;
            return erase(position, next);
        }

        iterator erase (iterator first, iterator last)
        {
            if (last == end()) {
                while (first != last) {
                    pop_back();
                    ++first;
                }
            }
            else {
                size_type count_deleted = 0;
                for (size_type i = first.get_index(); i < last.get_index(); ++i) {
                    _alloc.destroy(_element + i);
                    ++count_deleted;
                }
                for (size_type i = last.get_index(); i < _size; ++i) {
                    _alloc.construct(_element + first.get_index() + i - last.get_index(), _element [i]);
                    _alloc.destroy(_element + i);
                }
                _size -= count_deleted;
            }
            return iterator(first.get_index() - 1, _size, _element);
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
                x._alloc = tmp;
            }
        }

        void clear()
        {
            for (size_type i = 0; i < _size; ++i)
                _alloc.destroy(_element + i);
            _size = 0;
        }

    private:
        size_type _size;
        size_type _capacity;
        value_type * _element;
        allocator_type _alloc;

        void push_back_n (size_type n, const value_type & val = value_type())
        {
            size_type new_size = _size + n;
            if (new_size > _capacity) {
                value_type * new_element = realloc(new_size);
                _alloc.deallocate(_element, _size);
                _element = new_element;
            }
            for (size_type i = _size; i < new_size; ++i)
                _alloc.construct(_element + i, val);
            _size = new_size;
        }

        // leave container in a valid state by
        // using tmp capacity
        value_type * realloc(size_type new_size)
        {
            _capacity == 0 ? _capacity = 1 : 0;
            while (new_size > _capacity)
                _capacity = ceil(_capacity * 1.5);
            value_type * new_element = _alloc.allocate(_capacity);
            for (size_type i = 0; i < _size; ++i) {
                _alloc.construct(new_element + i, _element[i]);
                _alloc.destroy(_element + i);
            }
            return new_element;
        }

}; // class vector

template <class T, class Alloc>
void swap (vector<T,Alloc>& x, vector<T,Alloc>& y) { x.swap(y); }

} // namespace ft
