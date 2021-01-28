#pragma once

#include <cstddef>

namespace ft {

template <class T>
struct list_node
{
        T content;
        list_node * prev;
        list_node * next;

        list_node() : content(T()), prev(NULL), next(NULL) {};
        list_node(T c) : content(c), prev(NULL), next(NULL) {};
        list_node(T c, list_node * p, list_node * n) : content(c), prev(p), next(n) {};
        //~list_node() { delete next; }
        ~list_node() {}
};

} // namespace ft
