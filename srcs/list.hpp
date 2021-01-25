#pragma once

# include <iostream>

namespace ft {

template < class T > struct Node;
template < class T > class list_iterator;

// use template parameter bool is_const ?
//template < class value_type >
//class list_iterator
//{
//    public:
//        typedef std::bidirectional_iterator_tag iterator_category;

//        list_iterator ( ft::list<value_type> & list )
//            : _list(list) {};
//    private:
//        ft::list<value_type> & _list;
//};

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
        //typedef list_iterator<value_type> iterator;
        //typedef list_iterator<const value_type> const_iterator;
        //typedef reverse_iterator
        //typedef const_reverse_iterator
        //typedef difference_type
        typedef size_t size_type;

        /* Constructors */
        explicit list (const allocator_type& alloc = allocator_type())
            : _head(NULL), _size(0) {}

        explicit list (size_t n, const value_type& val = value_type(),
                const allocator_type& alloc = allocator_type())
            : _head(NULL), _size(n)
        {
            for (size_type i = 0; i < _size; ++i)
                push_front(val);
        }

        //substitution failure with second constructor
        // use std::is_integral ?
        // check sfinae cpp reference page
        // https://stackoverflow.com/questions/45889587/c-sfinae-to-differentiate-fill-and-range-constructors
        template <class InputIterator, typename std::iterator_traits<InputIterator>::value_type>
        list (InputIterator first, InputIterator last,
                const allocator_type& alloc = allocator_type()) {}

        list (const list& x);

        /* Operators */
        list &operator=(const list&);

        /* Destructor */
        ~list() {}

        /* Iterators */
        //iterator begin();
        //const_iterator begin() const;
        //iterator end();
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
        reference front ();
        const_reference front () const;
        reference back ();
        const_reference back () const;

        /* Modifiers */
        template <class InputIterator>
        void assign (InputIterator first, InputIterator last);
        void assign (size_type n, const value_type& val);
        void push_front (const value_type& val)
        {
            Node<value_type> * node = new Node<value_type>(val);

            if (!_head)
                _head = node;
            else {
                node->next = _head;
                _head->prev = node;
                _head = node;
            }
        }
        void pop_front ()
        {
            if (!_head)
                return ;
            Node<value_type> * tmp = _head;
            _head = _head->next;
            --_size;
            delete tmp;
        }
        void push_back (const value_type& val);
        void pop_back ();
        //iterator insert (iterator position, const value_type& val);
        //void insert (iterator position, size_type n, const value_type& val);
        //template <class InputIterator>
        //void insert (iterator position, InputIterator first, InputIterator last);
        //iterator erase (iterator position);
        //iterator erase (iterator first, iterator last);
        void swap (list& x);
        void resize (size_type n, value_type val = value_type());
        void clear ();

        /* Operations */
        //void splice (iterator position, list& x);
        //void splice (iterator position, list& x, iterator i);
        //void splice (iterator position, list& x, iterator first, iterator last);
        void remove (const value_type& val);
        template <class Predicate>
        void remove_if (Predicate pred);
        void unique();
        template <class BinaryPredicate>
        void unique (BinaryPredicate binary_pred);
        void merge (list& x);
        template <class Compare>
        void merge (list& x, Compare comp);
        void sort();
        template <class Compare>
        void sort (Compare comp);
        void reverse();

    private:
        Node<value_type> * _head;
        size_type _size;

}; // class list

template <class T, class Alloc>
bool operator== (const list<T,Alloc>& lhs, const list<T,Alloc>& rhs);
template <class T, class Alloc>
bool operator!= (const list<T,Alloc>& lhs, const list<T,Alloc>& rhs);
template <class T, class Alloc>
bool operator<  (const list<T,Alloc>& lhs, const list<T,Alloc>& rhs);
template <class T, class Alloc>
bool operator<= (const list<T,Alloc>& lhs, const list<T,Alloc>& rhs);
template <class T, class Alloc>
bool operator>  (const list<T,Alloc>& lhs, const list<T,Alloc>& rhs);
template <class T, class Alloc>
bool operator>= (const list<T,Alloc>& lhs, const list<T,Alloc>& rhs);

template <class T, class Alloc>
void swap (list<T,Alloc>& x, list<T,Alloc>& y);

template < class T >
struct Node
{
    T content;
    Node * next;
    Node * prev;

    Node(T contentt) : content(contentt), next(NULL), prev(NULL) {};
};

} // namespace ft
