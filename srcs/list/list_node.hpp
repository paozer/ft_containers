#pragma once

#include <cstddef>

namespace ft {

template <class T>
struct list_node
{
        T content;
        list_node * prev;
        list_node * next;

        list_node(T c = T(), list_node * p = NULL, list_node * n = NULL)
            : content(c), prev(p), next(n) {};

        ~list_node() {}
}; // struct list_node

} // namespace ft
