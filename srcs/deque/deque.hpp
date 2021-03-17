#pragma once

#include "deque_iterator.hpp"
#include "../utils/utils.hpp"

#include "../vector/vector.hpp"

#include <cstddef> // std::ptrdiff_t, size_t, NULL

#include <memory> // std::allocator
#include <stdexcept> // std::out_of_range
#include <limits> // std::numeric_limits

namespace ft {

template < class T, class Alloc = std::allocator<T> >
class deque
{
    static const size_t chunk_size = 32;

    public:
        typedef T value_type;
        typedef Alloc allocator_type;
        typedef typename allocator_type::reference reference;
        typedef typename allocator_type::const_reference const_reference;
        typedef typename allocator_type::pointer pointer;
        typedef typename allocator_type::const_pointer const_pointer;
        typedef deque_iterator<value_type, chunk_size, false> iterator;
        typedef deque_iterator<value_type, chunk_size, true> const_iterator;
        typedef reverse_deque_iterator<value_type, chunk_size, false> reverse_iterator;
        typedef reverse_deque_iterator<value_type, chunk_size, true> const_reverse_iterator;
        typedef std::ptrdiff_t difference_type;
        typedef size_t size_type;

    private:
        typedef deque<value_type, allocator_type> self_type;
        typedef typename allocator_type::template rebind<value_type*>::other ptr_allocator_type;
        typedef value_type** map_pointer;

    public:
        /* CONSTRUCTORS */
        explicit deque (const allocator_type& alloc = allocator_type())
            : _alloc(alloc), _ptr_alloc(alloc)
        {
            initialize_map();
        }

        explicit deque (size_type n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type())
            : _alloc(alloc), _ptr_alloc(alloc)
        {
            initialize_map();
            assign(n, val);
        }

        template <class InputIterator>
        deque (InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type(),
              typename ft::enable_if< !std::numeric_limits<InputIterator>::is_integer , void >::type* = 0)
            : _alloc(alloc), _ptr_alloc(alloc)
        {
            initialize_map();
            assign(first, last);
        }

        deque (const deque& x)
            : _alloc(x._alloc), _ptr_alloc(x._ptr_alloc)
        {
            initialize_map();
            *this = x;
        }

        /* DESTRUCTOR */
        ~deque()
        {
            clear();
            for (size_type i = 0; i < _map_size; ++i)
                _alloc.deallocate(_map[i], chunk_size);
            _ptr_alloc.deallocate(_map, _map_size);
        }

        /* OPERATORS */
        deque& operator= (const deque& x)
        {
            if (this != &x) {
                assign(x.begin(), x.end());
            }
            return *this;
        }

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

        /* ITERATORS */
        iterator begin() { return _first + 1; }
        iterator end() { return _last; }
        const_iterator end() const { return _last; }
        reverse_iterator rend() { return reverse_iterator(_first.get_curr(), _first.get_map()); }
        const_reverse_iterator rend() const { return const_reverse_iterator(_first.get_curr(), _first.get_map()); }
        reverse_iterator rbegin() { return reverse_iterator((_last - 1).get_curr(), (_last - 1).get_map()); }

        const_iterator begin() const
        {
            const_iterator it = _first;
            return ++it;
        }

        const_reverse_iterator rbegin() const
        {
            const_iterator it = _last;
            --it;
            return const_reverse_iterator(it.get_curr(), it.get_map());
        }

        /* CAPACITY */
        size_type size() const { return _size; }
        size_type max_size() const { return _alloc.max_size(); }
        bool empty() const { return _size == 0; }

        void resize (size_type n, value_type val = value_type())
        {
            if (_size < n) {
                while (_size < n)
                    push_back(val);
            } else {
                while (n < _size)
                    pop_back();
            }
        }

        /* ELEMENT ACCESS */
        reference front () { return _first[1]; }
        const_reference front () const { return begin()[0]; }
        reference back () { return _last[-1]; }
        const_reference back () const { return end()[-1]; }

        reference at (size_type n)
        {
            if (n >= _size)
                throw std::out_of_range("ft::deque");
            return _first[n + 1];
        }

        const_reference at (size_type n) const
        {
            if (n >= _size)
                throw std::out_of_range("ft::deque");
            return begin()[n];
        }

        reference operator[] (size_type n) { return _first[n + 1]; }
        const_reference operator[] (size_type n) const { return begin()[n]; }

        /* MODIFIERS */
        template <class InputIterator>
        void assign (InputIterator first, InputIterator last,
              typename ft::enable_if< !std::numeric_limits<InputIterator>::is_integer , void >::type* = 0)
        {
            clear();
            for (; first != last; ++first)
                push_back(*first);
        }

        void assign(size_type n, const value_type& val)
        {
            clear();
            for (size_type i = 0; i < n; ++i)
                push_back(val);
        }

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
            // and there is no empty chunk succeeding it
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
            ++_first;
            _alloc.destroy(_first.get_curr());
            --_size;
        }

        void pop_back (void)
        {
            --_last;
            _alloc.destroy(_last.get_curr());
            --_size;
        }

        // INSERT
        iterator insert (iterator position, const value_type& val)
        {
            difference_type i = position - begin();
            insert(position, 1, val);
            return begin() + i;
        }

        void insert (iterator position, size_type n, const value_type& val)
        {
            ft::vector<value_type> tmp (position, end());
            erase(position, end());
            for (size_type i = 0; i < n; ++i)
                push_back(val);
            for (typename ft::vector<value_type>::iterator it = tmp.begin(); it != tmp.end(); ++it)
                push_back(*it);
        }

        template <class InputIterator>
        void insert (iterator position, InputIterator first, InputIterator last,
              typename ft::enable_if< !std::numeric_limits<InputIterator>::is_integer , void >::type* = 0)
        {
            ft::vector<value_type> tmp (position, end());
            erase(position, end());
            for (; first != last; ++first)
                push_back(*first);
            for (typename ft::vector<value_type>::iterator it = tmp.begin(); it != tmp.end(); ++it)
                push_back(*it);
        }

        // ERASE
        iterator erase (iterator position)
        {
            return erase(position, position + 1);
        }

        iterator erase (iterator first, iterator last)
        {
            _size -= last - first;
            iterator tmp = first;
            difference_type offset = first - begin();

            for (; tmp != last; ++tmp)
                _alloc.destroy(tmp.get_curr());

            if (last - end() < first - begin()) { // shift succeeding elements to the left
                for (iterator it = last; it != end(); ++it, ++first) {
                    _alloc.construct(first.get_curr(), *it);
                    _alloc.destroy(it.get_curr());
                }
                _last = _first + _size + 1;
                _first = _last - _size - 1;
            } else { // shift preceding elements to the right
                tmp = first;
                for (iterator it = begin(); it != first; ++it, ++tmp) {
                    _alloc.construct(tmp.get_curr(), *it);
                    _alloc.destroy(it.get_curr());
                }
                _first = _last - _size - 1;
                _last = _first + _size + 1;
            }
            return begin() + offset;
        }

        void swap (deque& x)
        {
            ft::swap(_map, x._map);
            ft::swap(_first, x._first);
            ft::swap(_last, x._last);
            ft::swap(_size, x._size);
            ft::swap(_map_size, x._map_size);
            ft::swap(_alloc, x._alloc);
            ft::swap(_ptr_alloc, x._ptr_alloc);
            //ft::swap(_chunk_size, x._chunk_size);
        }

        void clear(void)
        {
            iterator ite = end();
            for (iterator it = begin(); it != ite; ++it)
                _alloc.destroy(it.get_curr());
            _first = iterator(_map[_map_size / 2] + (chunk_size / 2), _map + _map_size / 2);
            _last = _first + 1;
            _size = 0;
        }

    private:
        map_pointer _map;
        iterator _first;
        iterator _last;
        size_type _size;
        size_type _map_size;
        allocator_type _alloc;
        ptr_allocator_type _ptr_alloc;

        void realloc_map (bool at_front)
        {
            map_pointer tmp = _ptr_alloc.allocate(_map_size + 1);
            if (at_front) { // add an empty chunk at the beginning of the map

                // + 1 to account for added array
                difference_type offset = _last.get_map() - _map + 1;
                tmp[0] = _alloc.allocate(chunk_size);
                for (size_t i = 0; i < _map_size; ++i)
                    tmp[i + 1] = _map[i];
                _first.set_map(tmp);
                _first.set_curr(*tmp + chunk_size - 1);
                _last.set_map(tmp + offset);

            } else { // add an empty chunk at the end of the map

                difference_type offset = _first.get_map() - _map;
                tmp[_map_size] = _alloc.allocate(chunk_size);
                for (size_t i = 0; i < _map_size; ++i)
                    tmp[i] = _map[i];
                _first.set_map(tmp + offset);
                _last.set_map(tmp + _map_size);
                _last.set_curr(tmp[_map_size]);

            }
            _ptr_alloc.deallocate(_map, _map_size);
            _map = tmp;
            ++_map_size;
        }

        void initialize_map (void)
        {
            _size = 0;
            _map_size = 8;
            _map = _ptr_alloc.allocate(_map_size);
            for (size_type i = 0; i < _map_size; ++i)
                _map[i] = _alloc.allocate(chunk_size);
            _first = iterator(_map[_map_size / 2] + (chunk_size / 2), _map + _map_size / 2);
            _last = _first + 1;
        }

}; // CLASS DEQUE

template <class T, class Alloc>
void swap (deque<T, Alloc>& x, deque<T, Alloc>& y)
{
    x.swap(y);
}

} // NAMESPACE FT
