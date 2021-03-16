#pragma once

#include <cstddef> // NULL

namespace ft {

template <class T>
struct avl_node
{
    T content;
    int height;
    avl_node * left;
    avl_node * right;
    avl_node * parent;

    avl_node (const T& cont = T())
        : content(cont), height(0), left(NULL), right(NULL), parent(NULL)
    {
    }

}; // STRUCT AVL_NODE

} // NAMESPACE FT
