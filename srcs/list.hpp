#pragma once

#include <iostream>
#include "list_node.hpp"
#include "list_iterator.hpp"

namespace ft {

template < class T, class Alloc = std::allocator<T> >
class list
{

    public:
        typedef T value_type;
        typedef Alloc allocator_type;
        typedef typename allocator_type::reference reference;
        typedef typename allocator_type::const_reference const_reference;
        typedef typename allocator_type::pointer pointer;
        typedef typename allocator_type::const_pointer const_pointer;

        typedef list_iterator<value_type> iterator;
        //typedef list_iterator<const value_type> const_iterator;
        //typedef reverse_iterator
        //typedef const_reverse_iterator
        //typedef difference_type
        typedef size_t size_type;

        /* Constructors */
        explicit list (const allocator_type& alloc = allocator_type())
            : _head(new list_node<value_type>()), _tail(new list_node<value_type>()), _size(0)
        {
            _head->next = _tail;
            _tail->prev = _head;
        }

        explicit list (size_t n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type())
            : _head(new list_node<value_type>()), _tail(new list_node<value_type>()), _size(0)
        {
            _head->next = _tail;
            _tail->prev = _head;
            for (size_type i = 0; i < n; ++i)
                push_front(val);
        }

        // use std::is_integral ?
        template <class InputIterator, typename std::iterator_traits<InputIterator>::value_type>
        list (InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type())
            : _head(new list_node<value_type>()), _tail(new list_node<value_type>()), _size(0)
        {
            _head->next = _tail;
            _tail->prev = _head;
            for (; first != last; ++first)
                push_back(*first);
        }

        list (const list& x)
            : _head(new list_node<value_type>()), _tail(new list_node<value_type>()), _size(0)
        {
            *this = x;
        }

        /* Operators */
        list &operator=(const list& x)
        {
            if (*this != x)
                assign(x.begin(), x.end());
            return (*this);
        }

        // use iterators vs pointers ?
        bool operator== (const list<value_type, allocator_type>& rhs)
        {
            if (_size != rhs._size)
                return (false);
            list_node<T> * lhs_node = _head;
            list_node<T> * rhs_node = rhs._head;
            while (lhs_node && rhs_node) {
                if (lhs_node->content != rhs_node->content)
                    return (false);
                lhs_node = lhs_node->next;
                rhs_node = rhs_node->next;
            }
            return (true);

        }

        bool operator!= (const list<value_type, allocator_type>& rhs) { return (!(*this == rhs)); }

        bool operator< (const list<value_type, allocator_type>& rhs)
        {
            list_node<T> * lhs_node = _head;
            list_node<T> * rhs_node = rhs._head;
            while (lhs_node && rhs_node) {
                if (lhs_node->content < rhs_node->content)
                    return (true);
                else if (lhs_node->content > rhs_node->content)
                    return (false);
                lhs_node = lhs_node->next;
                rhs_node = rhs_node->next;
            }
            if (_size >= rhs._size)
                return false;
            return true;
        }

        bool operator<= (const list<value_type, allocator_type>& rhs) { return (*this == rhs || *this < rhs); }
        bool operator> (const list<value_type, allocator_type>& rhs) { return (!(*this < rhs) && *this != rhs); }
        bool operator>= (const list<value_type, allocator_type>& rhs) { return (*this == rhs || *this > rhs); }

        /* Destructor */
        ~list()
        {
            for (list_node<value_type> * tmp; _head; _head = tmp) {
                tmp = _head->next;
                delete _head;
            }
        }

        /* Iterators */
        iterator begin() { return (iterator(_head->next)); }
        //const_iterator begin() const;
        iterator end() { return (iterator(_tail)); }
        //const_iterator end() const;
        //reverse_iterator rbegin();
        //const_reverse_iterator rbegin() const;
        //reverse_iterator rend();
        //const_reverse_iterator rend() const;

        /* Capacity */
        bool empty() const { return (_size == 0); }
        size_type size() const { return (_size); }
        size_type max_size() const { return (std::numeric_limits<size_type>::max()); }

        /* Element Access */
        reference front () { return (*(_head->next)); }
        const_reference front () const { return (*(_head->next)); }
        reference back () { return (*(_tail->prev)); }
        const_reference back () const { return (*(_tail->prev)); }

        /* Modifiers */
        template <class InputIterator>
        void assign (InputIterator first, InputIterator last)
        {
            clear();
            for (; first != last; ++first)
                push_back(*first);
        }

        void assign (size_type n, const value_type& val)
        {
            clear();
            for (size_type i = 0; i < n; ++i)
                push_front(val);
        }

        void push_front (const value_type& val)
        {
            list_node<value_type> * node = new list_node<value_type>(val);
            node->next = _head->next;
            node->prev = _head;
            _head->next->prev = node;
            _head->next = node;
            ++_size;
        }

        void pop_front (void)
        {
            if (_size == 0)
                return ;
            list_node<value_type> * tmp = _head->next;
            _head->next->next->prev = _head;
            _head->next = _head->next->next;
            --_size;
            delete tmp;
        }

        void push_back (const value_type& val)
        {
            list_node<value_type> * node = new list_node<value_type>(val);
            node->next = _tail;
            node->prev = _tail->prev;
            node->prev->next = node;
            _tail->prev = node;
            ++_size;
        }

        void pop_back (void)
        {
            if (_size == 0)
                return ;
            list_node<value_type> * tmp = _tail->prev;
            _tail->prev->prev->next = _tail;
            _tail->prev = _tail->prev->prev;
            --_size;
            delete tmp;
        }

        iterator insert (iterator position, const value_type& val)
        {
            list_node<value_type> * to_insert = new list_node<value_type> (val, position.get_prev(), position.get_node());
            position.get_prev()->next = to_insert;
            position.get_node()->prev = to_insert;
            ++_size;
            return (--position);
        }

        void insert (iterator position, size_type n, const value_type& val)
        {
            for (size_type i = 0; i < n; ++i)
                insert(position, val);
        }

        template <class InputIterator>
        void insert (iterator position, InputIterator first, InputIterator last)
        {
            for (; first != last; ++first)
                insert(position, *first);
        }

        iterator erase (iterator position)
        {
            position.get_next()->prev = position.get_prev();
            position.get_prev()->next = position.get_next();
            iterator ret = iterator (position.get_prev()->next);
            delete position.get_node();
            --_size;
            return (ret);
        }

        iterator erase (iterator first, iterator last)
        {
            for (; first != last; ++first)
                erase(first);
        }

        void swap (list& x)
        {
            {
                size_type tmp = _size;
                _size = x._size;
                x._size = tmp;
            }
            {
                list_node<value_type> * tmp = _head;
                _head = x._head;
                x._head = tmp;
                tmp = _tail;
                _tail = x._tail;
                x._tail = tmp;
            }
        }

        void resize (size_type n, value_type val = value_type())
        {
            if (n < _size) {
                while (n < _size)
                    pop_back();
            }
            else if (n > _size) {
                while (n > _size)
                    push_back(val);
            }
        }

        void clear (void)
        {
            list_node<value_type> * node = _head->next;
            for (; node != _tail; node = node->next)
                delete node;
            _size = 0;
        }

        /* Operations */
        void splice (iterator position, list& x)
        {
            splice(position, x, x.begin(), x.end());
        }

        void splice (iterator position, list& x, iterator i)
        {
            if (x._size == 0)
                return ;
            i.get_prev()->next = i.get_next();
            i.get_next()->prev = i.get_prev();

            i.get_node()->next = position.get_node();
            i.get_node()->prev = position.get_prev();
            position.get_prev()->next = i.get_node();
            position.get_node()->prev = i.get_node();
            ++_size;
            --x._size;
        }

        void splice (iterator position, list& x, iterator first, iterator last)
        {
            first.get_prev()->next = last.get_node();
            last.get_node()->prev = first.get_prev(); // diff than splice w/ iter i and no last ?

            while (first != last) {
                iterator tmp (first.get_next());
                first.get_node()->next = position.get_node();
                first.get_node()->prev = position.get_prev();
                position.get_prev()->next = first.get_node();
                position.get_node()->prev = first.get_node();
                first = tmp;
                ++_size;
                --x._size;
            }
        }

        void remove (const value_type& val)
        {
            for (iterator it ; it != end(); ++it) {
                if (*it == val)
                    it = erase(it);
            }
        }

        template <class Predicate>
        void remove_if (Predicate pred)
        {
            for (iterator it = begin(); it != end(); ++it) {
                if (pred(*it))
                    erase(it);
            }
        }

        void unique()
        {
            unique(std::equal_to<value_type>());
        }

        template <class BinaryPredicate>
        void unique (BinaryPredicate binary_pred)
        {
            if (_size < 2)
                return ;
            iterator it = begin();
            iterator prev = it++;
            for (; it != end(); ++it) {
                if (binary_pred(*it, *prev))
                    it = erase(it);
                else
                    prev = it;
            }
        }

        void merge (list& x)
        {
            merge(x, std::less<value_type>());
        }

        template <class Compare>
        void merge (list& x, Compare comp)
        {
            if (x._size == 0)
                return ;

            list<value_type> result;
            while (_size != 0 && x._size != 0) {
                if (comp(*begin(), *x.begin()))
                    result.splice(result.end(), *this, begin());
                else
                    result.splice(result.end(), x, x.begin());
            }
            while (_size != 0)
                result.splice(result.end(), *this, begin());
            while (x._size != 0)
                result.splice(result.end(), x, x.begin());
            _size = result._size;

            _head->next = result._head->next;
            _head->next->prev = _head;
            _tail->prev = result._tail->prev;
            _tail->prev->next = _tail;

            result._head->next = result._tail;
            result._tail->prev = result._head;
        }

        void sort()
        {
            sort(std::less<value_type>());
        }

        template <class Compare>
        void sort (Compare comp)
        {
            if (_size < 2)
                return ;

            list<value_type> left;
            list<value_type> right;
            size_type i = 0;
            for (iterator it = begin(); it != end(); it = begin()) {
                if (i < _size / 2)
                    left.splice(left.begin(), *this, it);
                else
                    right.splice(right.begin(), *this, it);
                ++i;
            }
            left.sort(comp);
            right.sort(comp);
            left.merge(right);
            merge(left);
        }

        void reverse()
        {
            if (_size < 2)
                return ;
            ft::list<int>::iterator pos = begin();
            for (size_type i = 0; i < _size - 1; ++i)
                splice(pos, *this, --end());
        }

    private:
        list_node<value_type> * _head;
        list_node<value_type> * _tail;
        size_type _size;

}; // class list

template <class T, class Alloc>
void swap (list<T,Alloc>& x, list<T,Alloc>& y)
{
    x.swap(y);
}

} // namespace ft
