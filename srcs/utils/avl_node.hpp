#pragma once

#include <cstddef> // NULL
#include <utility> // std::pair

namespace ft {

template <class Key, class Value>
struct avl_node
{
    std::pair<const Key, Value> pair;
    int bf;
    avl_node * left;
    avl_node * right;
    avl_node * parent;

    avl_node (std::pair<Key, Value> pair)
        : pair(pair), bf(0), left(NULL), right(NULL), parent(NULL)
    {
    }

}; // STRUCT MAP_NODE

} // NAMESPACE FT
