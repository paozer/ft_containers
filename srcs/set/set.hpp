#pragma once

#include "../utils/avl_tree.hpp"
#include "../utils/avl_iterator.hpp"
#include "../utils/utils.hpp" // ft::enable_if

#include <cstddef> // std::ptddiff_t, size_t

#include <memory> // std::allocator
#include <limits> // std::numeric_limits
#include <functional> // std::less

namespace ft {

template < class T,
           class Compare = std::less<T>,
           class Alloc = std::allocator<T>
           >
class set : public avl_tree<T, Compare, Alloc>
{
    public:
        typedef T key_type;
        typedef T value_type;
        typedef Compare key_compare;
        typedef Compare value_compare;
        typedef Alloc allocator_type;
        typedef typename allocator_type::reference reference;
        typedef typename allocator_type::const_reference const_reference;
        typedef typename allocator_type::pointer pointer;
        typedef typename allocator_type::const_pointer const_pointer;
        typedef avl_iterator<value_type, false> iterator;
        typedef avl_iterator<value_type, true> const_iterator;
        typedef reverse_avl_iterator<value_type, false> reverse_iterator;
        typedef reverse_avl_iterator<value_type, true> const_reverse_iterator;
        typedef std::ptrdiff_t difference_type;
        typedef size_t size_type;

    private:
        typedef avl_tree<value_type, key_compare, allocator_type> Base;

    public:
        /* CONSTRUCTORS */
        explicit set (const value_compare& comp = value_compare(), const allocator_type& alloc = allocator_type())
            : _comp(comp), _alloc(alloc)
        {
        }

        template <class InputIterator>
        set (InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type(),
              typename ft::enable_if< !std::numeric_limits<InputIterator>::is_integer , void >::type* = 0)
            : _comp(comp), _alloc(alloc)
        {
            for (; first != last; ++first)
                Base::insert(*first);
        }

        set (const set& x)
            : _comp(x._comp), _alloc(x._alloc)
        {
            *this = x;
        }

        /* DESTRUCTOR */
        ~set()
        {
        }

        /* OPERATORS */
        set& operator= (const set& x)
        {
            if (this != &x) {
                _comp = x._comp;
                Base::clear();
                for (const_iterator it = x.begin(); it != x.end(); ++it)
                    Base::insert(*it);
            }
            return *this;
        }

        /* OBSERVERS */
        key_compare key_comp (void) const { return _comp; }
        value_compare value_comp (void) const { return value_compare(_comp); }

    private:
        value_compare _comp;
        allocator_type _alloc;

}; // CLASS MAP

} // NAMESPACE FT
