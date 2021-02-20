#include <utility>

namespace ft {

enum color_t { RED, BLACK };

template <class Key, class Value>
struct rb_node
{
    std::pair<Key, Value> p;
    rb_node * left;
    rb_node * right;
    rb_node * parent;
    enum color_t color;

    rb_node (std::pair<Key, Value> pair)
        : p(pair), left(NULL), right(NULL), parent(NULL), color(RED)
    {
    }

    void switch_color()
    {
        if (color == RED)
            color = BLACK;
        else
            color = RED;
    }

}; // STRUCT MAP_NODE

} // NAMESPACE FT
