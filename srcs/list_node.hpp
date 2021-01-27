#pragma once

#include <cstddef>

namespace ft {

template <class T>
struct list_node
{
        T _content;
        list_node * next;
        list_node * prev;

        list_node() : _content(T()), next(NULL), prev(NULL) {};
        list_node(T content) : _content(content), next(NULL), prev(NULL) {};
        ~list_node() {}
};

} // namespace ft
