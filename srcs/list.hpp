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

        //list (const list& x);

        /* Operators */
        //list &operator=(const list&);

        /* Destructor */
        ~list() {}

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
        //reference front ();
        //const_reference front () const;
        //reference back ();
        //const_reference back () const;

        /* Modifiers */
        //template <class InputIterator>
        //void assign (InputIterator first, InputIterator last);
        //void assign (size_type n, const value_type& val);

        void push_front (const value_type& val)
        {
            list_node<value_type> * node = new list_node<value_type>(val);
            node->next = _head->next;
            node->prev = _head;
            _head->next->prev = node;
            _head->next = node;
            ++_size;
        }
        //void pop_front (void);
        void push_back (const value_type& val)
        {
            list_node<value_type> * node = new list_node<value_type>(val);
            node->next = _tail;
            node->prev = _tail->prev;
            node->prev->next = node;
            _tail->prev = node;
            ++_size;
        }
        //void pop_back ();
        //iterator insert (iterator position, const value_type& val);
        //void insert (iterator position, size_type n, const value_type& val);
        //template <class InputIterator>
        //void insert (iterator position, InputIterator first, InputIterator last);
        //iterator erase (iterator position);
        //iterator erase (iterator first, iterator last);
        //void swap (list& x);
        //void resize (size_type n, value_type val = value_type());
        //void clear ();

        /* Operations */
        //void splice (iterator position, list& x);
        //void splice (iterator position, list& x, iterator i);
        //void splice (iterator position, list& x, iterator first, iterator last);
        //void remove (const value_type& val);
        //template <class Predicate>
        //void remove_if (Predicate pred);
        //void unique();
        //template <class BinaryPredicate>
        //void unique (BinaryPredicate binary_pred);
        //void merge (list& x);
        //template <class Compare>
        //void merge (list& x, Compare comp);
        //void sort();
        //template <class Compare>
        //void sort (Compare comp);
        //void reverse();


    private:
        list_node<value_type> * _head;
        list_node<value_type> * _tail;
        size_type _size;

}; // class list

//template <class T, class Alloc>
//bool operator== (const list<T,Alloc>& lhs, const list<T,Alloc>& rhs);
//template <class T, class Alloc>
//bool operator!= (const list<T,Alloc>& lhs, const list<T,Alloc>& rhs);
//template <class T, class Alloc>
//bool operator<  (const list<T,Alloc>& lhs, const list<T,Alloc>& rhs);
//template <class T, class Alloc>
//bool operator<= (const list<T,Alloc>& lhs, const list<T,Alloc>& rhs);
//template <class T, class Alloc>
//bool operator>  (const list<T,Alloc>& lhs, const list<T,Alloc>& rhs);
//template <class T, class Alloc>
//bool operator>= (const list<T,Alloc>& lhs, const list<T,Alloc>& rhs);
//
//template <class T, class Alloc>
//void swap (list<T,Alloc>& x, list<T,Alloc>& y);

} // namespace ft
