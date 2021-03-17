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

        reference operator[] (size_type n)
        {
            self_type tmp = *this;
            tmp += n;
            return *tmp;
        }

        friend bool operator== (const self_type& lhs, const self_type& rhs) { return lhs._curr == rhs._curr; }
        friend bool operator!= (const self_type& lhs, const self_type& rhs) { return !(lhs == rhs); }
        friend bool operator< (const self_type& lhs, const self_type& rhs) { return rhs - lhs > 0; }
        friend bool operator> (const self_type& lhs, const self_type& rhs) { return rhs < lhs; }
        friend bool operator<= (const self_type& lhs, const self_type& rhs) { return !(rhs < lhs); }
        friend bool operator>= (const self_type& lhs, const self_type& rhs) { return !(lhs < rhs); }

        /* ITERATOR SUBTRACTION */
        friend difference_type operator- (const self_type& lhs, const self_type& rhs)
        {
            difference_type n = 0;

            if (lhs._map < rhs._map) {

                n += lhs.distance_2_end();                      // lhs distance to end
                n += (rhs._map - lhs._map - 1) * chunk_size;  // map distance difference
                n += rhs.distance_2_start();                    // rhs distance to start
                n += 1;
                n *= -1;

            } else if (lhs._map > rhs._map) {

                n += rhs.distance_2_end();
                n += (lhs._map - rhs._map - 1) * chunk_size;
                n += lhs.distance_2_start();
                n += 1;

            } else {
                n = lhs._curr - rhs._curr;
            }
            return n;
        }

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

        self_type operator++ (int)
        {
            self_type tmp = *this;
            ++tmp;
            return tmp;
        }

        self_type operator-- (int)
        {
            self_type tmp = *this;
            --tmp;
            return tmp;
        }

        /* ITERATOR & INTEGER ARITHMETIC */
        self_type& operator+= (difference_type n)
        {
            difference_type distance_to_start = distance_2_start();
            difference_type distance_to_end = distance_2_end();

            if (n > 0 && n > distance_to_end) {
                _map += (n + distance_to_start) / chunk_size;
                _curr = *_map + (n - distance_to_end - 1) % chunk_size;
            } else if (n < 0 && n < -distance_to_start) {
                _map -= (distance_to_end - n) / chunk_size;
                _curr = *_map + chunk_size - 1 - (distance_to_end - n) % chunk_size;
            } else {
                _curr += n;
            }
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
            return tmp += -n;
        }

        friend self_type operator+ (difference_type n, const self_type& rhs)
        {
            self_type tmp = rhs;
            return tmp += n;
        }

        /* GETTERS */
        pointer get_curr (void) const { return _curr; }
        map_pointer get_map (void) const { return _map; }

        /* SETTERS */
        void set_curr (pointer curr) { _curr = curr; }
        void set_map (map_pointer map) { _map = map; }

        /* UTILITIES */
        bool is_last (void) const { return _curr == *_map + chunk_size - 1; }
        bool is_first (void) const { return _curr == *_map; }

    private:
        pointer _curr;
        map_pointer _map;

        difference_type distance_2_start (void) const { return _curr - *_map; }
        difference_type distance_2_end (void) const { return *_map + chunk_size - 1 - _curr; }

}; // CLASS DEQUE_ITERATOR

template <class T, size_t chunk_size, bool is_const>
class reverse_deque_iterator
{
    public:
        typedef T value_type;
        typedef size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef typename choose<is_const, const T *, T *>::type pointer;
        typedef typename choose<is_const, const T &, T &>::type reference;
        typedef std::random_access_iterator_tag iterator_category;

    private:
        typedef reverse_deque_iterator<value_type, chunk_size, is_const> self_type;
        typedef value_type** map_pointer;

    public:
        reverse_deque_iterator (pointer curr = NULL, map_pointer map = NULL)
            : _curr(curr), _map(map)
        {
        }

        reverse_deque_iterator (const reverse_deque_iterator<value_type, chunk_size, false>& other)
            : _curr(other.get_curr()), _map(other.get_map())
        {
        }

        /* OPERATORS */
        reverse_deque_iterator &operator= (const self_type& other)
        {
            if (this != &other) {
                _curr = other._curr;
                _map = other._map;
            }
            return *this;
        }

        pointer operator-> (void) { return _curr; }
        reference operator* (void) { return *_curr; }

        reference operator[] (size_type n)
        {
            self_type tmp = *this;
            tmp += n;
            return *tmp;
        }

        friend bool operator== (const self_type& lhs, const self_type& rhs) { return lhs._curr == rhs._curr; }
        friend bool operator!= (const self_type& lhs, const self_type& rhs) { return !(lhs == rhs); }
        friend bool operator< (const self_type& lhs, const self_type& rhs) { return rhs - lhs > 0; }
        friend bool operator> (const self_type& lhs, const self_type& rhs) { return rhs < lhs; }
        friend bool operator<= (const self_type& lhs, const self_type& rhs) { return !(rhs < lhs); }
        friend bool operator>= (const self_type& lhs, const self_type& rhs) { return !(lhs < rhs); }

        /* ITERATOR SUBTRACTION */
        friend difference_type operator- (const self_type& lhs, const self_type& rhs)
        {
            difference_type n = 0;

            if (lhs._map < rhs._map) {

                n += lhs.distance_2_end();                      // lhs distance to end
                n += (rhs._map - lhs._map - 1) * chunk_size;  // map distance difference
                n += rhs.distance_2_start();                    // rhs distance to start
                n += 1;

            } else if (lhs._map > rhs._map) {

                n += rhs.distance_2_end();
                n += (lhs._map - rhs._map - 1) * chunk_size;
                n += lhs.distance_2_start();
                n += 1;
                n *= -1;

            } else {
                n = rhs._curr - lhs._curr;
            }
            return n;
        }

        /* INCREMENTING & DECREMENTING */
        self_type& operator++ (void)
        {
            if (is_first()) {
                --_map;
                _curr = *_map + chunk_size - 1;
            } else {
                --_curr;
            }
            return *this;
        }

        self_type& operator-- (void)
        {
            if (is_last()) {
                ++_map;
                _curr = *_map;
            } else {
                ++_curr;
            }
            return *this;
        }

        self_type operator++ (int)
        {
            self_type tmp = *this;
            --tmp;
            return tmp;
        }

        self_type operator-- (int)
        {
            self_type tmp = *this;
            ++tmp;
            return tmp;
        }

        /* ITERATOR & INTEGER ARITHMETIC */
        self_type& operator+= (difference_type n)
        {
            difference_type distance_to_start = distance_2_start();
            difference_type distance_to_end = distance_2_end();

            if (n > 0 && n > distance_to_start) {
                _map -= (n + distance_to_end) / chunk_size;
                _curr = *_map + chunk_size - (n - distance_to_start) % chunk_size;
            } else if (n < 0 && n < -distance_to_end) {
                _map += (distance_to_start - n) / chunk_size;
                _curr = *_map + (1 - n - distance_to_end) % chunk_size;
            } else {
                _curr -= n;
            }
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
            return tmp += -n;
        }

        friend self_type operator+ (difference_type n, const self_type& rhs)
        {
            self_type tmp = rhs;
            return tmp += n;
        }

        /* GETTERS */
        pointer get_curr (void) const { return _curr; }
        map_pointer get_map (void) const { return _map; }

        /* SETTERS */
        void set_curr (pointer curr) { _curr = curr; }
        void set_map (map_pointer map) { _map = map; }

        /* UTILITIES */
        bool is_last (void) const { return _curr == *_map + chunk_size - 1; }
        bool is_first (void) const { return _curr == *_map; }

    private:
        pointer _curr;
        map_pointer _map;

        difference_type distance_2_start (void) const { return _curr - *_map; }
        difference_type distance_2_end (void) const { return *_map + chunk_size - 1 - _curr; }

}; // CLASS DEQUE_ITERATOR

} // NAMESPACE FT
