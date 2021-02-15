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
    node_s * prev;
    enum color_t color;

    node_s (T c = T())
        : content(c), left(NULL), right(NULL), prev(NULL), color(RED)
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
class LLRB
{
    public:
        typedef T value_type;
        typedef node_s<value_type> node;
        typedef node * node_pointer;
        typedef Compare value_compare;
        typedef Alloc allocator_type;
        typedef size_t size_type;

    private:
        typedef typename Alloc::template rebind<node>::other node_allocator;

    public:
        LLRB (const value_compare& comp = value_compare(), const allocator_type& alloc = allocator_type())
            : _root(NULL), _size(0), _added_node_flag(false), _added_node_ptr(NULL),
              _low_node(NULL), _high_node(NULL), _alloc(alloc), _comp(comp)
        {
        }

        ~LLRB()
        {
            clear();
        }

        /* ELEMENT ACCESS */
        node_pointer get_min(void) { return _low_node; }
        node_pointer get_max(void) { return _high_node; }

        /* CAPACITY */

        size_type size(void) const { return _size; }
        bool empty(void) { return _size == 0; }
        size_type max_size(void) { return _alloc.max_size(); }

        /* MODIFIERS */

        std::pair<node_pointer, bool> insert(const value_type& val)
        {
            _root = insert(_root, NULL, val);
            _root->color = BLACK;
            if (_added_node_flag)
                ++_size;
            return std::make_pair(_added_node_ptr, _added_node_flag);
        }

        // position gives a hint to where the new node could be placed
        // if hint is correct insert recursively from here
        // else insert recursively from root
        node_pointer insert (node_pointer position, const value_type& val)
        {
            if (_comp(val, position->content))
                position->left = insert(position->left, position, val);
            else
                _root = insert(_root, NULL, val);
            _root->color = BLACK;
            if (_added_node_flag)
                ++_size;
            return insert(val).first;
        }

        void clear(void)
        {
            node_pointer tmp = _root;
            _root = NULL;
            _low_node = NULL;
            _high_node = NULL;
            _size = 0;
            _clear(tmp);
        }

        /* OPERATIONS */

        node_pointer find(value_type val)
        {
            node_pointer x = _root;
            while (x != NULL) {
                int cmp = _comp(x->content, val);
                if (cmp == 0)
                    return x;
                else if (cmp > 0)
                    x = x->right;
                else
                    x = x->left;
            }
            return NULL;
        }

        /* DEBUG */

        void print_min_max(void)
        {
            std::cout << "low node " << (_low_node ? std::to_string(_low_node->content.first) : "") << "\n";
            std::cout << "high node " << (_high_node ? std::to_string(_high_node->content.first) : "") << "\n";
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
            //std::cout << "node [" << n->content << "]\n";
            std::cout << "node [" << n->content.first << "]" << " left [" << (n->left ? std::to_string(n->left->content.first) : "") << "] right [" << (n->right ? std::to_string(n->right->content.first) : "") << "]";
            //std::cout << "content [" << n->content.second << "]\n";
            std::cout << "\n";
            print_tree(n->right);
        }

    private:
        node_pointer _root;
        size_type _size;

        bool _added_node_flag;
        node_pointer _added_node_ptr;
        node_pointer _low_node;
        node_pointer _high_node;

        node_allocator _alloc;
        value_compare _comp;

        /* MODIFIERS */

        node_pointer insert (node_pointer node, node_pointer prev, value_type val) {
            if (!node) {
                node_pointer p = new_node(val);
                p->prev = prev;
                // need to remember node in order to be create iterator
                _added_node_ptr = p;
                _added_node_flag = true;
                // update low and high nodes
                if (!_low_node || _comp(val, _low_node->content))
                    _low_node = p;
                if (!_high_node || _comp(_high_node->content, val))
                    _high_node = p;
                return p;
            }

            if (_comp(val, node->content))
                node->left = insert(node->left, node, val);
            else if (_comp(node->content, val))
                node->right = insert(node->right, node, val);
            else {
                // need to remember node in order to be create iterator
                _added_node_ptr = node;
                _added_node_flag = false;
            }
            node = fix(node);
            return node;
        }

        void _clear(node_pointer node)
        {
            if (!node)
                return ;
            _clear(node->left);
            _clear(node->right);
            delete_node(node);
        }

        /* UTILITIES */

        node_pointer fix(node_pointer n)
        {
            //fix right-leaning red node
            if (is_red(n->right) && !is_red(n->left))
                n = rotate_left(n);
            //fix two red ns in a row
            if (is_red(n->left) && is_red(n->left->left))
                n = rotate_right(n);
            // split 4-n
            if (is_red(n->left) && is_red(n->right))
                flip_colors(n);
            return n;
        }

        void flip_colors (node_pointer n)
        {
            n->switch_color();
            n->left->switch_color();
            n->right->switch_color();
        }

        /*
         * Rotates node to it's left (right) subtree.
         * The root of it's right (left) subtree takes its place.
         * The new root is returned.
         */

        node_pointer rotate_left (node_pointer root)
        {
            node_pointer right_root = root->right;

            root->right = right_root->left;
            root->right ? root->right->prev = root : 0;

            right_root->left = root;
            right_root->prev = root->prev;
            root->prev = right_root;

            right_root->color = root->color;
            root->color = RED;
            return right_root;
        }

        node_pointer rotate_right (node_pointer root)
        {
            node_pointer left_root = root->left;

            root->left = left_root->right;
            root->left ? root->left->prev = root : 0;

            left_root->right = root;
            left_root->prev = root->prev;
            root->prev = left_root;

            left_root->color = root->color;
            root->color = RED;
            return left_root;
        }

        bool is_red(node_pointer n)
        {
            if (!n)
                return false;
            return n->color == RED;
        }

        /* MEMORY MANAGEMENT */

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

}; // class LLRB

} // namespace ft
