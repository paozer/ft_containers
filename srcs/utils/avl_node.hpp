#pragma once

#include <utility>
#include <cstddef>

namespace ft {

enum color_t { RED, BLACK };

template <class Key, class Value>
struct avl_node
{
    std::pair<const Key, Value> pair;
    avl_node * left;
    avl_node * right;
    avl_node * parent;

    avl_node (std::pair<Key, Value> pair)
        : pair(pair), left(NULL), right(NULL), parent(NULL)
    {
    }

}; // struct map_node

} // namespace ft
