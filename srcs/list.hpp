#pragma once

#include <iostream>
#include <memory>
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

        typedef list_iterator<value_type, false> iterator;
        typedef list_iterator<value_type, true> const_iterator;
        typedef reverse_list_iterator<value_type, false> reverse_iterator;
        typedef reverse_list_iterator<value_type, true> const_reverse_iterator;
        //typedef difference_type
        typedef size_t size_type;

    private:
        typedef list_node<value_type> node;
        typedef node * node_pointer;
        typedef typename Alloc::template rebind<node>::other node_allocator;

    public:
        /* Constructors */
        explicit list (const allocator_type& alloc = allocator_type())
            : _size(0), _head(new_node()), _tail(new_node())
        {
            _head->next = _tail;
            _tail->prev = _head;
        }

        explicit list (size_t n, const value_type & val = value_type(), const allocator_type& alloc = allocator_type())
            : _size(0), _head(new_node()), _tail(new_node())
        {
            _head->next = _tail;
            _tail->prev = _head;
            for (size_type i = 0; i < n; ++i)
                push_front(val);
        }

        // use std::is_integral ?
        template <class InputIterator, typename std::iterator_traits<InputIterator>::value_type>
        list (InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type())
            : _size(0), _head(new_node()), _tail(new_node())
        {
            _head->next = _tail;
            _tail->prev = _head;
            for (; first != last; ++first)
                push_back(*first);
        }

        list (const list & x)
            : _size(0), _head(new_node()), _tail(new_node())
        {
            *this = x;
        }

        /* Operators */
        list &operator=(const list & x)
        {
            if (*this != x)
                assign(x.begin(), x.end());
            return *this;
        }

        // use iterators vs pointers ?
        bool operator== (const list<value_type, allocator_type>& rhs)
        {
            if (_size != rhs._size)
                return false;
            node_pointer lhs_node = _head;
            node_pointer rhs_node = rhs._head;
            while (lhs_node && rhs_node) {
                if (lhs_node->content != rhs_node->content)
                    return false;
                lhs_node = lhs_node->next;
                rhs_node = rhs_node->next;
            }
            return true;

        }

        bool operator!= (const list<value_type, allocator_type>& rhs) { return !(*this == rhs); }

        bool operator< (const list<value_type, allocator_type>& rhs)
        {
            node_pointer lhs_node = _head;
            node_pointer rhs_node = rhs._head;
            while (lhs_node && rhs_node) {
                if (lhs_node->content < rhs_node->content)
                    return true;
                else if (lhs_node->content > rhs_node->content)
                    return false;
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
            for (node_pointer tmp; _head; _head = tmp) {
                tmp = _head->next;
                delete_node(_head);
            }
        }

        /* Iterators */
        iterator begin() { return iterator(_head->next); }
        const_iterator begin() const { return const_iterator(_head->next); }
        iterator end() { return iterator(_tail); }
        const_iterator end() const { return const_iterator(_tail); }

        reverse_iterator rbegin() { return reverse_iterator(_tail->prev); }
        const_reverse_iterator rbegin() const { return const_reverse_iterator(_tail->prev); }
        reverse_iterator rend() { return reverse_iterator(_head); }
        const_reverse_iterator rend() const { return const_reverse_iterator(_head); }

        /* Capacity */
        bool empty() const { return _size == 0; }
        size_type size() const { return _size; }
        size_type max_size() const { return std::numeric_limits<size_type>::max(); }

        /* Element Access */
        reference front () { return _head->next->content; }
        const_reference front () const { return _head->next->content; }
        reference back () { return _tail->prev->content; }
        const_reference back () const { return _tail->prev->content; }

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
            node_pointer node = new_node(val);
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
            node_pointer tmp = _head->next;
            _head->next->next->prev = _head;
            _head->next = _head->next->next;
            --_size;
            delete_node(tmp);
        }

        void push_back (const value_type& val)
        {
            node_pointer node = new_node(val);
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
            node_pointer tmp = _tail->prev;
            _tail->prev->prev->next = _tail;
            _tail->prev = _tail->prev->prev;
            --_size;
            delete_node(tmp);
        }

        iterator insert (iterator position, const value_type& val)
        {
            node_pointer to_insert = new_node();
            to_insert->prev = position.get_prev();
            to_insert->next = position.get_node();
            position.get_prev()->next = to_insert;
            position.get_node()->prev = to_insert;
            ++_size;
            return --position;
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
            iterator ret = iterator (position.get_next());
            delete_node(position.get_node());
            --_size;
            return ret;
        }

        iterator erase (iterator first, iterator last)
        {
            for (; first != last; ++first)
                erase(first);
        }

        void swap (list& x) // add swap of allocator ?
        {
            {
                size_type tmp = _size;
                _size = x._size;
                x._size = tmp;
            }
            {
                node_pointer tmp = _head;
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
            node_pointer node = _head->next;
            for (; node != _tail; node = node->next)
                delete_node(node);
            _size = 0;
        }

        /* Operations */
        void splice (iterator position, list& x)
        {
            splice(position, x, x.begin(), x.end());
        }

        // just call splice(position, x, i, ++i);
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
            last.get_node()->prev = first.get_prev();

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
            iterator prev = begin();
            iterator it = ++begin();
            for (; it != end(); ++it) {
                if (binary_pred(*it, *prev)) {
                    erase(it);
                    it = prev;
                }
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
            iterator pos = begin();
            for (size_type i = 0; i < _size - 1; ++i)
                splice(pos, *this, --end());
        }

    private:
        size_type _size;
        node_pointer _head;
        node_pointer _tail;
        node_allocator _alloc;

        node_pointer new_node(const value_type & val = value_type())
        {
            node_pointer p = _alloc.allocate(1);
            _alloc.construct(p, val);
            return p;
        }

        void delete_node(node_pointer p)
        {
            _alloc.destroy(p);
            _alloc.deallocate(p, 1);
        }

}; // class list

template <class T, class Alloc>
void swap (list<T,Alloc>& x, list<T,Alloc>& y)
{
    x.swap(y);
}

} // namespace ft
