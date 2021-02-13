#pragma once

#include <cstddef>

namespace ft {

template <class T>
struct map_node
{
        T content;
        map_node * left;
        map_node * right;

        map_node(T c = T(), map_node * p = NULL, map_node * n = NULL)
            : content(c), left(p), right(n)
        {
        }

        ~map_node()
        {
        }

}; // struct list_node

} // namespace ft
