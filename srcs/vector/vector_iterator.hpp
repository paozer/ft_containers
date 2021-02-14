#pragma once

#include "../utils/utils.hpp"
#include <iterator>

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
        vector_iterator(size_type index = 0, size_type size = 0, pointer ptr = NULL)
            : _index(index), _size(size), _ptr(ptr)
        {
        }

        vector_iterator (const vector_iterator<value_type, false>& other)
        {
            *this = other;
        }

        vector_iterator &operator=(const vector_iterator<value_type, is_const>& other)
        {
            if (this != &other) {
                _index = other._index;
                _size = other._size;
                _ptr = other._ptr;
            }
            return *this;
        }

        friend bool operator==(const self_type & lhs, const self_type & rhs)
        {
            if (lhs._ptr != rhs._ptr
                || lhs._size != rhs._size
                || lhs._index != rhs._index)
                return false;
            return true;
        }

        friend bool operator<(const self_type & lhs, const self_type & rhs)
        {
            if (lhs._ptr < rhs._ptr
                || lhs._size < rhs._size
                || lhs._index < rhs._index)
                return true;
            return false;
        }

        friend bool operator!=(const self_type & lhs, const self_type & rhs) { return !(lhs == rhs); }
        friend bool operator>(const self_type & lhs, const self_type & rhs) { return rhs < lhs; }
        friend bool operator<=(const self_type & lhs, const self_type & rhs) { return !(rhs < lhs); }
        friend bool operator>=(const self_type & lhs, const self_type & rhs) { return !(lhs < rhs); }

        pointer operator->(void)  { return &(_ptr + _index); }
        reference operator*(void) { return *(_ptr + _index); }
        self_type operator[](size_type n) { return *(_ptr + _index + n); }

        self_type & operator++(void)
        {
            ++_index;
            return *this;
        }
        self_type operator++(int)
        {
            self_type tmp = *this;
            ++_index;
            return tmp;
        }
        self_type & operator--(void)
        {
            --_index;
            return *this;
        }
        self_type operator--(int)
        {
            self_type tmp = *this;
            --_index;
            return tmp;
        }

        friend difference_type operator+(const self_type & lhs, const self_type & rhs) { return lhs._index + rhs._index; }
        friend difference_type operator-(const self_type & lhs, const self_type & rhs) { return lhs._index - rhs._index; }

        self_type operator+(difference_type n)
        {
            self_type tmp = *this;
            return tmp += n;
        }

        self_type operator-(difference_type n)
        {
            self_type tmp = *this;
            return tmp -= n;
        }

        self_type & operator+=(difference_type n)
        {
            _index += n;
            return *this;
        }

        self_type & operator-=(difference_type n) { return *this += -n; }

        size_type get_index() const { return _index; }

    private:
        size_t _index;
        size_t _size;
        pointer _ptr;

}; // class vector_iterator

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
        reverse_vector_iterator(size_type index = 0, size_type size = 0, pointer ptr = NULL)
            : _index(index), _size(size), _ptr(ptr)
        {
        }

        reverse_vector_iterator (const reverse_vector_iterator<value_type, false>& other)
        {
            *this = other;
        }

        reverse_vector_iterator &operator=(const vector_iterator<value_type, is_const>& other)
        {
            if (this != &other) {
                _index = other._index;
                _size = other._size;
                _ptr = other._ptr;
            }
            return *this;
        }

        friend bool operator==(const self_type & lhs, const self_type & rhs)
        {
            if (lhs._ptr != rhs._ptr
                || lhs._size != rhs._size
                || lhs._index != rhs._index)
                return false;
            return true;
        }

        friend bool operator<(const self_type & lhs, const self_type & rhs)
        {
            if (lhs._ptr < rhs._ptr
                || lhs._size < rhs._size
                || lhs._index < rhs._index)
                return true;
            return false;
        }

        friend bool operator!=(const self_type & lhs, const self_type & rhs) { return !(lhs == rhs); }
        friend bool operator>(const self_type & lhs, const self_type & rhs) { return rhs < lhs; }
        friend bool operator<=(const self_type & lhs, const self_type & rhs) { return !(rhs < lhs); }
        friend bool operator>=(const self_type & lhs, const self_type & rhs) { return !(lhs < rhs); }

        pointer operator->(void)  { return &(_ptr + _index - 1); }
        reference operator*(void) { return *(_ptr + _index - 1); }
        self_type operator[](size_type n) { return *(_ptr + _index - 1 + n); }

        self_type & operator++(void)
        {
            --_index;
            return *this;
        }
        self_type operator++(int)
        {
            self_type tmp = *this;
            --_index;
            return tmp;
        }
        self_type & operator--(void)
        {
            ++_index;
            return *this;
        }
        self_type operator--(int)
        {
            self_type tmp = *this;
            ++_index;
            return tmp;
        }

        friend difference_type operator+(const self_type & lhs, const self_type & rhs) { return lhs._index - rhs._index; }
        friend difference_type operator-(const self_type & lhs, const self_type & rhs) { return lhs._index + rhs._index; }

        self_type operator+(difference_type n)
        {
            self_type tmp = *this;
            return tmp -= n;
        }

        self_type operator-(difference_type n)
        {
            self_type tmp = *this;
            return tmp += n;
        }

        self_type & operator+=(difference_type n)
        {
            _index -= n;
            return *this;
        }

        self_type & operator-=(difference_type n) { return *this += n; }

        size_type get_index() const { return _index; }

    private:
        size_t _index;
        size_t _size;
        pointer _ptr;

}; // class reverse_vector_iterator

} // namespace ft
