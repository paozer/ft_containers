#pragma once

#include "../utils/utils.hpp"
#include <cstddef> // NULL, std::ptrdiff_t, size_t
#include <iterator> // std::random_access_iterator_tag

namespace ft {

template <class T, size_t chunk_size, bool is_const>
class deque_iterator
{
    public:
        typedef T value_type;
        typedef size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef typename choose<is_const, const T *, T *>::type pointer;
        typedef typename choose<is_const, const T &, T &>::type reference;
        typedef std::random_access_iterator_tag iterator_category;

    private:
        typedef deque_iterator<value_type, chunk_size, is_const> self_type;
        typedef value_type** map_pointer;

    public:
        deque_iterator (pointer curr = NULL, map_pointer map = NULL)
            : _curr(curr), _map(map)
        {
        }

        deque_iterator (const deque_iterator<value_type, chunk_size, false>& other)
            : _curr(other.get_curr()), _map(other.get_map())
        {
        }

        /* OPERATORS */
        deque_iterator &operator= (const self_type& other)
        {
            if (this != &other) {
                _curr = other._curr;
                _map = other._map;
            }
            return *this;
        }

        pointer operator-> (void) { return _curr; }
        reference operator* (void) { return *_curr; }
        //reference operator[] (size_type n);

        friend bool operator== (const self_type& lhs, const self_type& rhs) { return lhs._curr == rhs._curr; }
        friend bool operator!= (const self_type& lhs, const self_type& rhs) { return !(lhs == rhs); }
        //friend bool operator< (const self_type& lhs, const self_type& rhs) { return rhs - lhs > 0; }
        //friend bool operator> (const self_type& lhs, const self_type& rhs) { return rhs < lhs; }
        //friend bool operator<= (const self_type& lhs, const self_type& rhs) { return !(rhs < lhs); }
        //friend bool operator>= (const self_type& lhs, const self_type& rhs) { return !(lhs < rhs); }

        /* ITERATOR SUBTRACTION */
        //friend difference_type operator- (const self_type& lhs, const self_type& rhs);

        /* INCREMENTING & DECREMENTING */
        self_type& operator++ (void)
        {
            if (is_last()) {
                ++_map;
                _curr = *_map;
            } else {
                ++_curr;
            }
            return *this;
        }

        self_type& operator-- (void)
        {
            if (is_first()) {
                --_map;
                _curr = *_map + chunk_size - 1;
            } else {
                --_curr;
            }
            return *this;
        }

        self_type operator+ (difference_type n)
        {
            self_type tmp = *this;
            for (difference_type i = 0; i < n; ++i)
                ++tmp;
            return tmp;
        }

        self_type operator- (difference_type n)
        {
            self_type tmp = *this;
            for (difference_type i = 0; i < n; ++i)
                --tmp;
            return tmp;
        }

        /* UTILITIES */
        bool is_last (void) const { return _curr == *_map + chunk_size - 1; }
        bool is_first (void) const { return _curr == *_map; }

        /* GETTERS */
        pointer get_curr (void) const { return _curr; }
        map_pointer get_map (void) const { return _map; }

        /* SETTERS */
        void set_curr (pointer curr) { _curr = curr; }
        void set_map (map_pointer map) { _map = map; }

    private:
        pointer _curr;
        map_pointer _map;

}; // CLASS VECTOR_ITERATOR

} // NAMESPACE FT
