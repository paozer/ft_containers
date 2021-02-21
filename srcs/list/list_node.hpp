#pragma once

#include <cstddef> // NULL

namespace ft {

template <class T>
struct list_node
{
        T content;
        list_node * prev;
        list_node * next;

        list_node(T c = T(), list_node * p = NULL, list_node * n = NULL)
            : content(c), prev(p), next(n) {};

}; // STRUCT LIST_NODE

} // NAMESPACE FT
