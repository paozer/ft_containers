#pragma once

#include <memory>
#include <cstddef>

#include <iostream>

namespace ft {

enum color_t { RED, BLACK };

template <class T>
struct node_s
{
    T content;
    node_s * left;
    node_s * right;
    enum color_t color;

    node_s (T cont = T(), node_s * l = NULL, node_s * r = NULL)
        : content(cont), left(l), right(r), color(RED)
    {
    }

    void switch_color()
    {
        if (color == RED)
            color = BLACK;
        else
            color = RED;
    }
}; // struct node_s

template <class T, class Compare = std::less<T>, class Alloc = std::allocator<T> >
class leftLeaningRedBlackTree
{
    public:
        typedef leftLeaningRedBlackTree llrb;
        typedef T value_type;
        typedef node_s<value_type> node;
        typedef node * node_pointer;
        typedef Compare compare;
        typedef Alloc allocator_type;
        typedef size_t size_type;

    private:
        typedef typename Alloc::template rebind<node>::other node_allocator;

    public:
        leftLeaningRedBlackTree (const allocator_type& alloc = allocator_type())
            : _root(NULL), _size(0), _lowest_node(NULL),
              _highest_node(NULL), _alloc(alloc), _compare(compare())
        {
        }

        size_type size(void) { return _size; }

        void insert(value_type val)
        {
            _root = insert(_root, val);
            _root->color = BLACK;
            ++_size;
        }

        node_pointer find(value_type val)
        {
            node_pointer x = _root;
            while (x != NULL) {
                int cmp = _compare(x->content, val);
                if (cmp == 0)
                    return x;
                else if (cmp > 0)
                    x = x->right;
                else
                    x = x->left;
            }
            return NULL;
        }

        void print_tree(void)
        {
            print_tree(_root);
        }

        void print_tree(node_pointer n)
        {
            if (!n)
                return ;
            print_tree(n->left);
            if (n->left || n->right) {
                std::cout << "node [" << n->content << "] ";
                std::cout << "child left [" << (n->left ? std::to_string(n->left->content) : "  ") << "] ";
                std::cout << "child right [" << (n->right ? std::to_string(n->right->content) : "  ") << "] ";
                std::cout << "color [" << (is_red(n) ? " red ]\n" : "black]\n");
            }
            else {
                std::cout << "node [" << n->content << "] ";
                std::cout << "color [" << (is_red(n) ? " red ]\n" : "black]\n");
            }
            print_tree(n->right);
        }

    private:
        node_pointer _root;
        size_type _size;
        node_pointer _lowest_node;
        node_pointer _highest_node;
        node_allocator _alloc;
        compare _compare;

        node_pointer insert (node_pointer node, value_type val) {
            if (!node)
                return new_node(val);

            int cmp = _compare(node->content, val);
            if (cmp == 0)
                --_size;
            else if (cmp < 0)
                node->left = insert(node->left, val);
            else
                node->right = insert(node->right, val);
            node = fix(node);
            return node;
        }

        node_pointer fix(node_pointer node)
        {
            //if (is_red(node->right) && !is_red(node->left))
            //fix right-leaning red node
            if (is_red(node->right))
                node = rotate_left(node);
            //fix two red nodes in a row
            if (is_red(node->left) && is_red(node->left->left))
                node = rotate_right(node);
            // split 4-node
            if (is_red(node->left) && is_red(node->right))
                flip_colors(node);
            return node;
        }

        void flip_colors (node_pointer h)
        {
            h->switch_color();
            h->left->switch_color();
            h->right->switch_color();
        }

        node_pointer rotate_left (node_pointer h)
        {
            node_pointer x = h->right;
            h->right = x->left;
            x->left = h;
            x->color = h->color;
            h->color = RED;
            return x;
        }

        node_pointer rotate_right (node_pointer h)
        {
            node_pointer x = h->left;
            h->left = x->right;
            x->right = h;
            x->color = h->color;
            h->color = RED;
            return x;
        }

        bool is_red(node_pointer h)
        {
            if (!h)
                return false;
            return h->color == RED;
        }

        node_pointer new_node(const value_type& val = value_type())
        {
            node_pointer p = _alloc.allocate(1);
            _alloc.construct(p, val);
            return p;
        }

        void delete_node(node_pointer p)
        {
            _alloc.destroy(p);
            _alloc.deallocate(p, 1);
        }

}; // class leftLeaningRedBlackTree

} // namespace ft
