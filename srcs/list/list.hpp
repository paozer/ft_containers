#pragma once

#include "list_node.hpp"
#include "list_iterator.hpp"
#include "../utils/utils.hpp"

#include <cstddef> // NULL, std::ptrdiff_t

#include <memory> // std::allocator
#include <limits> // std::numeric_limits
#include <functional> // std::equal_to
#include <iterator> // std::distance

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
        typedef std::ptrdiff_t difference_type;
        typedef size_t size_type;

    private:
        typedef list_node<value_type> node;
        typedef node* node_pointer;
        typedef typename Alloc::template rebind<node>::other node_allocator;

    public:
        /* CONSTRUCTORS */
        explicit list (const allocator_type& alloc = allocator_type())
            : _size(0), _head(new_node()), _tail(new_node()), _alloc(alloc)
        {
            _head->next = _tail;
            _tail->prev = _head;
        }

        explicit list (size_t n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type())
            : _size(0), _head(new_node()), _tail(new_node()), _alloc(alloc)
        {
            _head->next = _tail;
            _tail->prev = _head;
            assign(n, val);
        }

        template <class InputIterator>
        list (InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type(),
              typename ft::enable_if< !std::numeric_limits<InputIterator>::is_integer , void >::type* = 0)
            : _size(0), _head(new_node()), _tail(new_node()), _alloc(alloc)
        {
            _head->next = _tail;
            _tail->prev = _head;
            assign(first, last);
        }

        list (const list& x)
            : _size(0), _head(new_node()), _tail(new_node()), _alloc(x._alloc)
        {
            _head->next = _tail;
            _tail->prev = _head;
            *this = x;
        }

        /* OPERATORS */
        list& operator= (const list& x)
        {
            if (this != &x)
                assign(x.begin(), x.end());
            return *this;
        }

        friend bool operator== (const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)
        {
            if (lhs._size != rhs._size)
                return false;
            const_iterator lit = lhs.begin();
            const_iterator rit = rhs.begin();
            while (lit != lhs.end() && rit != rhs.end()) {
                if (*lit != *rit)
                    return false;
                ++lit;
                ++rit;
            }
            return true;
        }

        friend bool operator< (const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)
        {
            const_iterator lit = lhs.begin();
            const_iterator rit = rhs.begin();
            while (lit != lhs.end() && rit != rhs.end()) {
                if (*lit < *rit)
                    return true;
                if (*lit > *rit)
                    return false;
                ++lit;
                ++rit;
            }
            if (lhs._size >= rhs._size)
                return false;
            return true;
        }

        friend bool operator!= (const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) { return !(lhs == rhs); }
        friend bool operator<= (const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) { return !(rhs < lhs); }
        friend bool operator> (const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) { return rhs < lhs; }
        friend bool operator>= (const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) { return !(lhs < rhs); }

        /* DESTRUCTOR */
        ~list()
        {
            for (node_pointer tmp; _head; _head = tmp) {
                tmp = _head->next;
                delete_node(_head);
            }
        }

        /* ITERATORS */
        iterator begin() { return iterator(_head->next); }
        const_iterator begin() const { return const_iterator(_head->next); }
        iterator end() { return iterator(_tail); }
        const_iterator end() const { return const_iterator(_tail); }
        reverse_iterator rbegin() { return reverse_iterator(_tail->prev); }
        const_reverse_iterator rbegin() const { return const_reverse_iterator(_tail->prev); }
        reverse_iterator rend() { return reverse_iterator(_head); }
        const_reverse_iterator rend() const { return const_reverse_iterator(_head); }

        /* CAPACITY */
        bool empty() const { return _size == 0; }
        size_type size() const { return _size; }
        size_type max_size() const { return _alloc.max_size(); }

        /* ELEMENT ACCESS */
        reference front (void) { return _head->next->content; }
        const_reference front (void) const { return _head->next->content; }
        reference back (void) { return _tail->prev->content; }
        const_reference back (void) const { return _tail->prev->content; }

        /* MODIFIERS */
        template <class InputIterator>
        void assign (InputIterator first, InputIterator last,
              typename ft::enable_if< !std::numeric_limits<InputIterator>::is_integer , void >::type* = 0)
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
            _tail->prev->next = node;
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
            node_pointer to_insert = new_node(val);
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
        void insert (iterator position, InputIterator first, InputIterator last,
              typename ft::enable_if< !std::numeric_limits<InputIterator>::is_integer , void >::type* = 0)
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
            while (first != last)
                first = erase(first);
            return last;
        }

        void swap (list& x)
        {
            ft::swap(_alloc, x._alloc);
            ft::swap(_size, x._size);
            ft::swap(_head, x._head);
            ft::swap(_tail, x._tail);
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
            if (_size == 0)
                return ;
            erase(begin(), end());
            _head->next = _tail;
            _tail->prev = _head;
            _size = 0;
        }

        /* OPERATIONS */
        void splice (iterator position, list& x)
        {
            splice(position, x, x.begin(), x.end());
        }

        void splice (iterator position, list& x, iterator i)
        {
            iterator next = i;
            ++next;
            splice(position, x, i, next);
        }

        void splice (iterator position, list& x, iterator first, iterator last)
        {
            if (first == last)
                return ;

            difference_type i = std::distance(first, last);
            _size += i;
            x._size -= i;

            node_pointer tmp = last.get_prev();
            first.get_prev()->next = last.get_node();
            last.get_node()->prev = first.get_prev();
            position.get_prev()->next = first.get_node();
            first.get_node()->prev = position.get_prev();
            tmp->next = position.get_node();
            position.get_node()->prev = tmp;
        }

        void remove (const value_type& val)
        {
            for (iterator it = begin(); it != end(); ++it) {
                if (*it == val) {
                    it = erase(it);
                    --it;
                }
            }
        }

        template <class Predicate>
        void remove_if (Predicate pred)
        {
            for (iterator it = begin(); it != end(); ++it) {
                if (pred(*it)) {
                    it = erase(it);
                    --it;
                }
            }
        }

        void unique (void)
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
            if (this == &x || x._size == 0)
                return ;

            list<value_type> result;
            while (_size != 0 && x._size != 0) {
                if (comp(*begin(), *x.begin()))
                    result.splice(result.end(), *this, begin());
                else
                    result.splice(result.end(), x, x.begin());
            }
            if (_size != 0)
                result.splice(result.end(), *this);
            if (x._size != 0)
                result.splice(result.end(), x);
            _size = result._size;

            _head->next = result._head->next;
            _head->next->prev = _head;
            _tail->prev = result._tail->prev;
            _tail->prev->next = _tail;

            result._head->next = result._tail;
            result._tail->prev = result._head;
        }

        void sort (void)
        {
            sort(std::less<value_type>());
        }

        template <class Compare>
        void sort (Compare comp)
        {
            if (_size < 2)
                return ;

            iterator it = begin();
            std::advance(it, _size / 2);

            list<value_type> left;
            list<value_type> right;
            left.splice(left.begin(), *this, begin(), it);
            right.splice(right.begin(), *this, begin(), end());

            left.sort(comp);
            right.sort(comp);
            left.merge(right, comp);
            merge(left, comp);
        }

        void reverse (void)
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

        /* MEMORY MANAGEMENT */
        node_pointer new_node (const value_type& val = value_type())
        {
            node_pointer p = _alloc.allocate(1);
            _alloc.construct(p, val);
            return p;
        }

        void delete_node (node_pointer p)
        {
            _alloc.destroy(p);
            _alloc.deallocate(p, 1);
        }

}; // CLASS LIST

template <class T, class Alloc>
void swap (list<T, Alloc>& x, list<T, Alloc>& y)
{
    x.swap(y);
}

} // NAMESPACE FT
