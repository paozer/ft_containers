#pragma once

#include <cstddef> // NULL

namespace ft {

template <class T>
struct list_node
{
        T content;
        list_node* prev;
        list_node* next;

        list_node (const T& cont = T())
            : content(cont), prev(NULL), next(NULL)
        {
        }

}; // STRUCT LIST_NODE

} // NAMESPACE FT
