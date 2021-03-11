#pragma once

#include "deque_iterator.hpp"
#include "../utils/utils.hpp"

#include <cstddef> // std::ptrdiff_t, size_t, NULL

#include <memory> // std::allocator
#include <stdexcept> // std::out_of_range
#include <limits> // std::numeric_limits

#define _CHUNK_SIZE_ 10

namespace ft {

template < class T, class Alloc = std::allocator<T> >
class deque
{
    public:
        typedef T value_type;
        typedef Alloc allocator_type;
        typedef typename allocator_type::reference reference;
        typedef typename allocator_type::const_reference const_reference;
        typedef typename allocator_type::pointer pointer;
        typedef typename allocator_type::const_pointer const_pointer;
        typedef deque_iterator<value_type, _CHUNK_SIZE_, false> iterator;
        typedef deque_iterator<value_type, _CHUNK_SIZE_, true> const_iterator;
        //typedef reverse_deque_iterator<value_type, _chunk_size, false> reverse_iterator;
        //typedef reverse_deque_iterator<value_type, _chunk_size, true> const_reverse_iterator;
        typedef std::ptrdiff_t difference_type;
        typedef size_t size_type;

    private:
        typedef deque<value_type, allocator_type> self_type;
        typedef typename allocator_type::template rebind<value_type*>::other ptr_allocator_type;
        typedef value_type** map_pointer;

    public:
        /* CONSTRUCTORS */
        explicit deque (const allocator_type& alloc = allocator_type())
            : _map(NULL), _size(0), _map_size(0), _alloc(alloc), _ptr_alloc(alloc)
        {
            realloc_map(true);
            _first  = iterator(*_map + _chunk_size / 2, _map);
            _last = _first + 1;
        }

        explicit deque (size_type n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type());
        template <class InputIterator>
        deque (InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type(),
              typename ft::enable_if< !std::numeric_limits<InputIterator>::is_integer , void >::type* = 0);
        deque (const deque& x);

        /* OPERATORS */
        deque& operator= (const deque& x);

        friend bool operator== (const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs)
        {
            if (lhs._size != rhs._size)
                return false;
            for (size_type i = 0; i < lhs._size; ++i) {
                if (lhs[i] != rhs[i])
                    return false;
            }
            return true;
        }

        friend bool operator< (const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs)
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

        friend bool operator!= (const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs) { return !(lhs == rhs); }
        friend bool operator<= (const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs) { return !(rhs < lhs); }
        friend bool operator> (const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs) { return rhs < lhs; }
        friend bool operator>= (const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs) { return !(lhs < rhs); }

        /* DESTRUCTOR */
        ~deque()
        {
            for (size_t i = 0; i < _map_size; ++i) {
                std::cout << "map [" << i << "] = ";
                for (size_t j = 0; j < _chunk_size; ++j) {
                    std::cout << _map[i][j] << " ";
                }
                std::cout << std::endl << std::endl;
            }
        }

        /* ITERATORS */
        iterator begin() { return _first + 1; }
        const_iterator begin() const { return _first + 1; }
        iterator end() { return _last; }
        const_iterator end() const { return _last; }
        //reverse_iterator rbegin() { return reverse_iterator(_size - 1, _array); }
        //const_reverse_iterator rbegin() const { return const_reverse_iterator(_size - 1, _array); }
        //reverse_iterator rend() { return reverse_iterator(-1, _array); }
        //const_reverse_iterator rend() const { return const_reverse_iterator(-1, _array); }

        /* CAPACITY */
        size_type size() const { return _size; }
        size_type max_size() const { return _alloc.max_size(); }
        bool empty() const { return _size == 0; }

        void resize (size_type n, value_type val = value_type());

        /* ELEMENT ACCESS */
        reference front () { return *(_first + 1); }
        const_reference front () const { return *(_first + 1); }
        reference back () { return *(_last - 1); }
        const_reference back () const { return *(_last - 1); }

        reference at (size_type n);
        const_reference at (size_type n) const;
        reference operator[] (size_type n);
        const_reference operator[] (size_type n) const;

        /* MODIFIERS */
        template <class InputIterator>
        void assign (InputIterator first, InputIterator last,
              typename ft::enable_if< !std::numeric_limits<InputIterator>::is_integer , void >::type* = 0);
        void assign(size_type n, const value_type& val);

        // PUSH N POP
        void push_front (const value_type& val)
        {
            // if current first chunk will be full after push_front
            // and there is no empty chunk preceding it
            if (_first.is_first() && _first.get_map() == _map) {
                pointer tmp = _first.get_curr();
                realloc_map(true);
                _alloc.construct(tmp, val);
            } else {
                _alloc.construct(_first.get_curr(), val);
                --_first;
            }
            ++_size;
        }

        void push_back (const value_type& val)
        {
            // if current last chunk will be full after push_back
            // and there is no empty chunk suceeding it
            if (_last.is_last() && _last.get_map() == _map + _map_size - 1) {
                pointer tmp = _last.get_curr();
                realloc_map(false);
                _alloc.construct(tmp, val);
            } else {
                _alloc.construct(_last.get_curr(), val);
                ++_last;
            }
            ++_size;
        }

        void pop_front (void)
        {
            if (_size > 0) {
                ++_first;
                _alloc.destroy(_first.get_curr());
                --_size;
            }
        }

        void pop_back (void)
        {
            if (_size > 0) {
                --_last;
                _alloc.destroy(_last.get_curr());
                --_size;
            }
        }

        // INSERT
        iterator insert (iterator position, const value_type& val);
        void insert (iterator position, size_type n, const value_type& val);
        template <class InputIterator>
        void insert (iterator position, InputIterator first, InputIterator last,
              typename ft::enable_if< !std::numeric_limits<InputIterator>::is_integer , void >::type* = 0);

        // ERASE
        iterator erase (iterator position);
        iterator erase (iterator first, iterator last);

        void swap (deque& x);
        void clear(void);

    private:
        map_pointer _map;
        iterator _first;
        iterator _last;

        size_type _size;
        size_type _map_size;
        static const size_type _chunk_size = _CHUNK_SIZE_;

        allocator_type _alloc;
        ptr_allocator_type _ptr_alloc;

        void realloc_map(bool at_front)
        {
            map_pointer tmp = _ptr_alloc.allocate(_map_size + 1);
            if (at_front) {
                tmp[0] = _alloc.allocate(_chunk_size);
                for (size_t i = 0; i < _map_size; ++i)
                    tmp[i + 1] = _map[i];
                _first.set_curr(tmp[0] + _chunk_size - 1);
            } else {
                tmp[_map_size] = _alloc.allocate(_chunk_size);
                for (size_t i = 0; i < _map_size; ++i)
                    tmp[i] = _map[i];
                _last.set_curr(tmp[_map_size]);
            }
            _first.set_map(tmp);
            _last.set_map(tmp + _map_size);
            _ptr_alloc.deallocate(_map, _map_size);
            _map = tmp;
            ++_map_size;
        }

}; // CLASS DEQUE

template <class T, class Alloc>
void swap (deque<T, Alloc>& x, deque<T, Alloc>& y);

} // NAMESPACE FT
