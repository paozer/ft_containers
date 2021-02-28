#pragma once

#include <cstddef> // NULL

namespace ft {

template <class T>
struct avl_node
{
    T content;
    int bf;
    avl_node * left;
    avl_node * right;
    avl_node * parent;

    avl_node (T cont = T())
        : content(cont), bf(0), left(NULL), right(NULL), parent(NULL)
    {
    }

}; // STRUCT AVL_NODE

} // NAMESPACE FT
