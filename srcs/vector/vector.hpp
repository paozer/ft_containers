#pragma once

#include "vector_iterator.hpp"
#include "../utils/utils.hpp"

#include <cstddef> // std::ptrdiff_t, size_t, NULL
#include <cmath> // ceil

#include <memory> // std::allocator
#include <stdexcept> // std::out_of_range
#include <limits> // std::numeric_limits

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
        /* CONSTRUCTORS */
        explicit vector (const allocator_type& alloc = allocator_type())
            : _size(0), _capacity(0), _array(NULL), _alloc(alloc)
        {
        }

        explicit vector (size_type n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type())
            : _size(0), _capacity(0), _array(NULL), _alloc(alloc)
        {
            push_back_n(n, val);
        }

        template <class InputIterator>
        vector (InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type(),
              typename ft::enable_if< !std::numeric_limits<InputIterator>::is_integer , void >::type* = 0)
            : _size(0), _capacity(0), _array(NULL), _alloc(alloc)
        {
            for (; first != last; ++first)
                push_back(*first);
        }

        vector (const vector& x)
            : _size(0), _capacity(0), _array(NULL), _alloc(x._alloc)
        {
            *this = x;
        }

        /* OPERATORS */
        vector& operator= (const vector& x)
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
                if (lhs[i] > rhs[i])
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

        /* DESTRUCTOR */
        ~vector()
        {
            clear();
            _alloc.deallocate(_array, _capacity);
        }

        /* ITERATORS */
        iterator begin() { return iterator(0, _size, _array); }
        const_iterator begin() const { return const_iterator(0, _size, _array); }
        iterator end() { return iterator(_size, _size, _array); }
        const_iterator end() const { return const_iterator(_size, _size, _array); }
        reverse_iterator rbegin() { return reverse_iterator(_size, _size, _array); }
        const_reverse_iterator rbegin() const { return const_reverse_iterator(_size, _size, _array); }
        reverse_iterator rend() { return reverse_iterator(0, _size, _array); }
        const_reverse_iterator rend() const { return const_reverse_iterator(0, _size, _array); }

        /* CAPACITY */
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
                value_type * new_array = realloc(n);
                _alloc.deallocate(_array, _size);
                _array = new_array;
            }
        }

        /* ELEMENT ACCESS */
        reference front () { return _array[0]; }
        const_reference front () const { return _array[0]; }
        reference back () { return _array[_size - 1]; }
        const_reference back () const { return _array[_size - 1]; }

        reference at (size_type n)
        {
            if (n >= _size)
                throw std::out_of_range("ft::vector");
            return _array[n];
        }

        const_reference at (size_type n) const
        {
            if (n >= _size)
                throw std::out_of_range("ft::vector");
            return _array[n];
        }

        reference operator[] (size_type n) { return _array[n]; }
        const_reference operator[] (size_type n) const { return _array[n]; }

        /* MODIFIERS */
        // use insert(begin(), first, last) ?
        template <class InputIterator>
        void assign (InputIterator first, InputIterator last,
              typename ft::enable_if< !std::numeric_limits<InputIterator>::is_integer , void >::type* = 0)
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
                _alloc.destroy(_array + _size);
            }
        }

        iterator insert (iterator position, const value_type& val)
        {
            insert(position, 1, val);
            return iterator(position.get_index() - 1, _size, _array);
        }

        void insert (iterator position, size_type n, const value_type& val)
        {
            if (position == end())
                push_back_n(n, val);
            else {
                size_type new_size = _size + n;
                bool needs_realloc = new_size > _capacity;
                value_type * tmp = _array;

                if (needs_realloc)
                    tmp = realloc(new_size);
                for (size_type i = _size - 1; position.get_index() + n < i; --i) {
                    _alloc.construct(tmp + n + i, _array[i]);
                    _alloc.destroy(_array + i);
                }
                for (size_type i = 0; i < n; ++i)
                    _alloc.construct(tmp + position.get_index() + i, val);
                if (needs_realloc) {
                    _alloc.deallocate(_array, _size);
                    _array = tmp;
                }
                _size = new_size;
            }
        }

        template <class InputIterator>
        void insert (iterator position, InputIterator first, InputIterator last,
              typename ft::enable_if< !std::numeric_limits<InputIterator>::is_integer , void >::type* = 0)
        {
            if (position == end()) {
                for (; first != last; ++first)
                    push_back(*first);
            }
            else {
                size_type n = std::distance(first, last);
                size_type new_size = _size + n;
                bool needs_realloc = new_size > _capacity;
                value_type * tmp = _array;

                if (needs_realloc)
                    tmp = realloc(new_size);
                size_type i = 0;
                for (iterator it = --end(); position + n < it; --it) {
                    _alloc.construct(tmp + n + i, *it);
                    _alloc.destroy(_array + it.get_index());
                    ++i;
                }
                for (size_type i = 0; first != last; ++i) {
                    _alloc.construct(tmp + position.get_index() + i, *first);
                    ++first;
                }
                if (needs_realloc) {
                    _alloc.deallocate(_array, _size);
                    _array = tmp;
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

        // first == begin ? valid return ?
        iterator erase (iterator first, iterator last)
        {
            if (first == last)
                return first;
            if (last == end()) {
                while (first != last) {
                    pop_back();
                    ++first;
                }
                return end();
            }
            else {
                size_type count_deleted = 0;
                for (size_type i = first.get_index(); i < last.get_index(); ++i) {
                    _alloc.destroy(_array + i);
                    ++count_deleted;
                }
                for (size_type i = last.get_index(); i < _size; ++i) {
                    _alloc.construct(_array + first.get_index() + i - last.get_index(), _array [i]);
                    _alloc.destroy(_array + i);
                }
                _size -= count_deleted;
            }
            return iterator(first.get_index(), _size, _array);
        }

        void swap (vector& x)
        {
            ft::swap(_size, x._size);
            ft::swap(_capacity, x._capacity);
            ft::swap(_array, x._array);
            ft::swap(_alloc, x._alloc);
        }

        void clear(void)
        {
            for (size_type i = 0; i < _size; ++i)
                _alloc.destroy(_array + i);
            _size = 0;
        }

    private:
        size_type _size;
        size_type _capacity;
        value_type * _array;
        allocator_type _alloc;

        // uses tmp to retain container validity if allocation throws
        void push_back_n (size_type n, const value_type & val = value_type())
        {
            size_type new_size = _size + n;
            if (new_size > _capacity) {
                value_type * new_array = realloc(new_size);
                _alloc.deallocate(_array, _size);
                _array = new_array;
            }
            for (size_type i = _size; i < new_size; ++i)
                _alloc.construct(_array + i, val);
            _size = new_size;
        }

        // uses tmp to retain container validity if allocation throws
        value_type * realloc(size_type new_size)
        {
            size_type new_capacity = (_capacity == 0 ? 1 : _capacity);
            while (new_size > new_capacity)
                new_capacity = ceil(new_capacity * 1.5);
            value_type * new_array = _alloc.allocate(new_capacity);
            for (size_type i = 0; i < _size; ++i) {
                _alloc.construct(new_array + i, _array[i]);
                _alloc.destroy(_array + i);
            }
            _capacity = new_capacity;
            return new_array;
        }

}; // CLASS VECTOR

template <class T, class Alloc>
void swap (vector<T, Alloc>& x, vector<T, Alloc>& y) { x.swap(y); }

} // NAMESPACE FT
