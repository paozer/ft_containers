#pragma once

#include "../utils/utils.hpp"
#include <cstddef> // NULL, std::ptrdiff_t, size_t
#include <iterator> // std::random_access_iterator_tag

namespace ft {

template <class T, bool is_const>
class vector_iterator
{
    public:
        typedef T value_type;
        typedef std::ptrdiff_t difference_type;
        typedef typename choose<is_const, const T *, T *>::type pointer;
        typedef typename choose<is_const, const T &, T &>::type reference;
        typedef std::random_access_iterator_tag iterator_category;

    private:
        typedef vector_iterator<value_type, is_const> self_type;
        typedef size_t size_type;

    public:
        vector_iterator (size_type index = 0, pointer array = NULL)
            : _index(index), _array(array)
        {
        }

        vector_iterator (const vector_iterator<value_type, false>& other)
            : _index(other.get_index()), _array(other.get_array())
        {
        }

        /* OPERATORS */
        vector_iterator &operator= (const self_type& other)
        {
            if (this != &other) {
                _index = other._index;
                _array = other._array;
            }
            return *this;
        }

        pointer operator-> (void) { return _array + _index; }
        reference operator* (void) { return *(_array + _index); }
        reference operator[] (size_type n) { return *(_array + _index + n); }

        friend bool operator== (const self_type& lhs, const self_type& rhs) { return rhs - lhs == 0; }
        friend bool operator< (const self_type& lhs, const self_type& rhs) { return rhs - lhs > 0; }
        friend bool operator!= (const self_type& lhs, const self_type& rhs) { return !(lhs == rhs); }
        friend bool operator> (const self_type& lhs, const self_type& rhs) { return rhs < lhs; }
        friend bool operator<= (const self_type& lhs, const self_type& rhs) { return !(rhs < lhs); }
        friend bool operator>= (const self_type& lhs, const self_type& rhs) { return !(lhs < rhs); }

        /* ITERATOR SUBTRACTION */
        friend difference_type operator- (const self_type& lhs, const self_type& rhs) { return lhs._index - rhs._index; }

        /* INCREMENTING & DECREMENTING */
        self_type& operator++ (void)
        {
            ++_index;
            return *this;
        }

        self_type operator++ (int)
        {
            self_type tmp = *this;
            ++_index;
            return tmp;
        }

        self_type& operator-- (void)
        {
            --_index;
            return *this;
        }

        self_type operator-- (int)
        {
            self_type tmp = *this;
            --_index;
            return tmp;
        }

        /* ITERATOR & INTEGER ARITHMETIC */
        self_type& operator+= (difference_type n)
        {
            _index += n;
            return *this;
        }

        self_type& operator-= (difference_type n)
        {
            return *this += -n;
        }

        self_type operator+ (difference_type n)
        {
            self_type tmp = *this;
            return tmp += n;
        }

        self_type operator- (difference_type n)
        {
            self_type tmp = *this;
            return tmp -= n;
        }

        friend self_type operator+ (difference_type n, const self_type& rhs)
        {
            self_type tmp = rhs;
            return tmp += n;
        }

        /* GETTERS */
        size_type get_index (void) const { return _index; }
        pointer get_array (void) const { return _array; }

    private:
        size_type _index;
        pointer _array;

}; // CLASS VECTOR_ITERATOR

template <class T, bool is_const>
class reverse_vector_iterator
{
    public:
        typedef T value_type;
        typedef std::ptrdiff_t difference_type;
        typedef typename choose<is_const, const T *, T *>::type pointer;
        typedef typename choose<is_const, const T &, T &>::type reference;
        typedef std::random_access_iterator_tag iterator_category;

    private:
        typedef reverse_vector_iterator<value_type, is_const> self_type;
        typedef size_t size_type;

    public:
        /* CONSTRUCTORS */
        reverse_vector_iterator (size_type index = 0, pointer array = NULL)
            : _index(index), _array(array)
        {
        }

        reverse_vector_iterator (const reverse_vector_iterator<value_type, false>& other)
            : _index(other.get_index()), _array(other.get_array())
        {
        }

        /* OPERATORS */
        reverse_vector_iterator& operator= (const vector_iterator<value_type, is_const>& other)
        {
            if (this != &other) {
                _index = other._index;
                _array = other._array;
            }
            return *this;
        }

        pointer operator-> (void) { return _array + _index; }
        reference operator* (void) { return *(_array + _index); }
        reference operator[] (size_type n) { return *(_array + _index - n); }

        friend bool operator== (const self_type& lhs, const self_type& rhs) { return rhs - lhs == 0; }
        friend bool operator< (const self_type& lhs, const self_type& rhs) { return rhs - lhs > 0; }
        friend bool operator!= (const self_type& lhs, const self_type& rhs) { return !(lhs == rhs); }
        friend bool operator> (const self_type& lhs, const self_type& rhs) { return rhs < lhs; }
        friend bool operator<= (const self_type& lhs, const self_type& rhs) { return !(rhs < lhs); }
        friend bool operator>= (const self_type& lhs, const self_type& rhs) { return !(lhs < rhs); }

        /* ITERATOR SUBTRACTION */
        friend difference_type operator- (const self_type& lhs, const self_type& rhs) { return rhs._index - lhs._index; }

        /* INCREMENTING & DECREMENTING */
        self_type& operator++ (void)
        {
            --_index;
            return *this;
        }

        self_type operator++ (int)
        {
            self_type tmp = *this;
            --_index;
            return tmp;
        }

        self_type& operator-- (void)
        {
            ++_index;
            return *this;
        }

        self_type operator-- (int)
        {
            self_type tmp = *this;
            ++_index;
            return tmp;
        }

        /* ITERATOR & INTEGER ARITHMETIC */
        self_type& operator+= (difference_type n)
        {
            _index -= n;
            return *this;
        }

        self_type& operator-= (difference_type n)
        {
            _index += n;
            return *this;
        }

        self_type operator+ (difference_type n)
        {
            self_type tmp = *this;
            return tmp += n;
        }

        self_type operator- (difference_type n)
        {
            self_type tmp = *this;
            return tmp -= n;
        }

        friend self_type operator+ (difference_type n, const self_type& rhs)
        {
            self_type tmp = rhs;
            return tmp += n;
        }

        /* GETTERS */
        size_type get_index (void) const { return _index; }
        pointer get_array (void) const { return _array; }

    private:
        size_type _index;
        pointer _array;

}; // CLASS REVERSE_VECTOR_ITERATOR

} // NAMESPACE FT
