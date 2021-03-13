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

    private:
        typedef vector<value_type, allocator_type> self_type;

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

        // TODO use range assign
        template <class InputIterator>
        vector (InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type(),
              typename ft::enable_if< !std::numeric_limits<InputIterator>::is_integer , void >::type* = 0)
            : _size(0), _capacity(0), _array(NULL), _alloc(alloc)
        {
            reserve(std::distance(first, last));
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
        iterator begin() { return iterator(0, _array); }
        const_iterator begin() const { return const_iterator(0, _array); }
        iterator end() { return iterator(_size, _array); }
        const_iterator end() const { return const_iterator(_size, _array); }
        reverse_iterator rbegin() { return reverse_iterator(_size - 1, _array); }
        const_reverse_iterator rbegin() const { return const_reverse_iterator(_size - 1, _array); }
        reverse_iterator rend() { return reverse_iterator(-1, _array); }
        const_reverse_iterator rend() const { return const_reverse_iterator(-1, _array); }

        /* CAPACITY */
        size_type size() const { return _size; }
        size_type max_size() const { return _alloc.max_size(); }

        void resize (size_type n, value_type val = value_type())
        {
            if (n > _size) {
                push_back_n(n - _size, val);
            } else {
                while (n < _size)
                    pop_back();
            }
        }

        size_type capacity() const { return _capacity; }
        bool empty() const { return _size == 0; }

        void reserve (size_type n)
        {
            if (n > _capacity)
                realloc(n);
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
        template <class InputIterator>
        void assign (InputIterator first, InputIterator last,
              typename ft::enable_if< !std::numeric_limits<InputIterator>::is_integer , void >::type* = 0)
        {
            clear();
            insert(begin(), first, last);
        }

        void assign(size_type n, const value_type& val)
        {
            clear();
            insert(begin(), n, val);
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
            return iterator(position.get_index(), _array);
        }

        void insert (iterator position, size_type n, const value_type& val)
        {
            self_type tmp (position, end());
            erase(position, end());
            reserve(_size + n + tmp._size);
            push_back_n(n, val);
            for (iterator it = tmp.begin(); it != tmp.end(); ++it)
                push_back(*it);
        }

        template <class InputIterator>
        void insert (iterator position, InputIterator first, InputIterator last,
              typename ft::enable_if< !std::numeric_limits<InputIterator>::is_integer , void >::type* = 0)
        {
            self_type tmp (position, end());
            erase(position, end());
            reserve(_size + std::distance(first, last) + tmp._size);
            for (; first != last; ++first)
                push_back(*first);
            for (iterator it = tmp.begin(); it != tmp.end(); ++it)
                push_back(*it);
        }


        iterator erase (iterator position)
        {
            iterator next = position;
            ++next;
            return erase(position, next);
        }

        iterator erase (iterator first, iterator last)
        {
            if (first == last)
                return first;
            vector tmp (last, end());
            size_type new_size = first - begin();
            while (_size > new_size)
                pop_back();
            for (iterator it = tmp.begin(); it != tmp.end(); ++it)
                push_back(*it);
            return iterator(first.get_index(), _array);
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

        void push_back_n (size_type n, const value_type & val = value_type())
        {
            reserve(_size + n);
            for (size_type i = 0; i < n; ++i)
                _alloc.construct(_array + _size + i, val);
            _size += n;
        }

        void realloc(size_type new_size)
        {
            size_type new_capacity = (_capacity == 0 ? 1 : _capacity);
            while (new_size > new_capacity)
                new_capacity = ceil(new_capacity * 1.5);
            value_type * new_array = _alloc.allocate(new_capacity);
            for (size_type i = 0; i < _size; ++i) {
                _alloc.construct(new_array + i, _array[i]);
                _alloc.destroy(_array + i);
            }
            _alloc.deallocate(_array, _capacity);
            _capacity = new_capacity;
            _array = new_array;
        }

}; // CLASS VECTOR

template <class T, class Alloc>
void swap (vector<T, Alloc>& x, vector<T, Alloc>& y)
{
    x.swap(y);
}

} // NAMESPACE FT
