#pragma once

namespace ft {

/* CHOOSE */
template <bool flag, class is_true, class is_false>
struct choose;

template <class is_true, class is_false>
struct choose<true, is_true, is_false>
{
    typedef is_true type;
};

template <class is_true, class is_false>
struct choose<false, is_true, is_false>
{
    typedef is_false type;
};

/* ENABLE */
template<bool Cond, class T = void>
struct enable_if
{
};

template<class T>
struct enable_if<true, T>
{
    typedef T type;
};

/* SWAP */
template <class T>
void swap ( T& a, T& b )
{
    T c (a);
    a = b;
    b = c;
}

template <class T>
T max (const T& a, const T& b)
{
    if (a >= b)
        return a;
    return b;
}

} // NAMESPACE FT
