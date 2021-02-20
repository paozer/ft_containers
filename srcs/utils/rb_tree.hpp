#pragma once

#include <memory> // std::allocator
#include <cstddef> // NULL, size_t
#include <utility> // std::pair
#include <functional> // std::less

#include "rb_node.hpp"

namespace ft {

template <class Key,
          class T,
          class Compare = std::less<Key>,
          class Alloc = std::allocator<std::pair<const Key, T> >
          >
class rb_tree
{
    public:
        typedef Key key_type;
        typedef T mapped_type;
        typedef std::pair<const key_type, mapped_type> value_type;
        typedef rb_node<key_type, mapped_type> node;
        typedef node * node_pointer;
        typedef Compare key_compare;
        typedef Alloc allocator_type;
        typedef size_t size_type;

    private:
        typedef typename Alloc::template rebind<node>::other node_allocator;

    public:
        rb_tree (const allocator_type& alloc = allocator_type())
            : _root(NULL), _size(0), _added_node_flag(false), _added_node_ptr(NULL),
              _begin(new_node()), _end(new_node()), _alloc(alloc), _comp(key_compare())
        {
            _begin->parent = _end;
            _end->parent = _begin;
        }

        ~rb_tree()
        {
            clear();
        }

        /* ELEMENT ACCESS */
        node_pointer begin(void) { return _begin->parent; }
        node_pointer end(void) { return _end; }
        node_pointer rbegin(void) { return _end->parent; }
        node_pointer rend(void) { return _begin; }

        /* CAPACITY */
        size_type size(void) const { return _size; }
        bool empty(void) const { return _size == 0; }
        size_type max_size(void) const { return _alloc.max_size(); }

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
            if (_comp(val, position->first))
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
            _size = 0;
            clear(tmp);
        }

        size_type erase (const key_type& key)
        {
            _begin->parent->left = NULL;
            _end->parent->right = NULL;
            _begin->parent = NULL;
            _end->parent = NULL;

            _root = erase(_root, key);
            _root->color = BLACK;

            _begin->parent = get_min(_root);
            _end->parent = get_max(_root);
            _begin->parent->left = _begin;
            _end->parent->right = _end;

            //fix return value
            return 0;
        }

        /* OPERATIONS */
        node_pointer find(const key_type& key)
        {
            node_pointer x = _root;
            while (x != NULL) {
                int cmp = _comp(x->p.first, key);
                if (cmp == 0)
                    return x;
                else if (cmp > 0)
                    x = x->right;
                else
                    x = x->left;
            }
            return NULL;
        }

    private:
        node_pointer _root;
        size_type _size;
        bool _added_node_flag;
        node_pointer _added_node_ptr;
        node_pointer _begin;
        node_pointer _end;
        node_allocator _alloc;
        key_compare _comp;

        /* MODIFIERS */
        node_pointer insert (node_pointer node, node_pointer parent, const value_type& val)
        {
            // if we're at a leaf (real or _begin/_end) insert the new node
            if (!node || node == _end || node == _begin) {
                node_pointer p = new_node(val);
                p->parent = parent;
                // update _begin/_end nodes
                if (_begin->parent == _end || _comp(val.first, _begin->parent->p.first)) {
                    _begin->parent = p;
                    p->left = _begin;
                }
                if (_end->parent == _begin || _comp(_end->parent->p.first, val.first)) {
                    _end->parent = p;
                    p->right = _end;
                }
                // need to remember node in order to create iterator after return from recursion
                _added_node_ptr = p;
                _added_node_flag = true;
                return p;
            }
            if (_comp(val.first, node->p.first))
                node->left = insert(node->left, node, val);
            else if (_comp(node->p.first, val.first))
                node->right = insert(node->right, node, val);
            else {
                // need to remember node in order to create iterator after return from recursion
                _added_node_ptr = node;
                _added_node_flag = false;
            }
            node = fix(node);
            return node;
        }

        node_pointer erase(node_pointer n, const key_type& key)
        {
            if (_comp(key, n->p.first)) {
                if (n->left) {
                    if (!is_red(n->left) && !is_red(n->left->left))
                        n = move_red_left(n);
                    n->left = erase(n->left, key);
                }
            }
            else {
                if (is_red(n->left))
                    n = rotate_right(n);
                if (!_comp(key, n->p.first) && !_comp(n->p.first, key) && !n->right) {
                    delete_node(n);
                    --_size;
                    return NULL;
                }
                if (n->right) {
                    if (!is_red(n->right) && !is_red(n->right->left))
                        n = move_red_right(n);
                    if (!_comp(key, n->p.first) && !_comp(n->p.first, key)) {
                        node_pointer min = get_min(n->right);
                        swap_nodes(n, min);
                        n = min;
                        n->right = erase_min(n->right);
                    }
                    else
                        n->right = erase(n->right, key);
                }
            }
            return fix(n);
        }

        void swap_nodes(node_pointer n1, node_pointer n2)
        {
            node_pointer parent1 = n1->parent;
            node_pointer left_n1 = n1->left;
            node_pointer right_n1 = n1->right;
            node_pointer parent2 = n2->parent;
            node_pointer left_n2 = n2->left;
            node_pointer right_n2 = n2->right;

            if (parent1) {
                if (parent1->left == n1)
                    parent1->left = n2;
                else
                    parent1->right = n2;
                n2->parent = parent1;
            }
            else {
                _root = n2;
                n2->parent = NULL;
            }

            n2->left = left_n1;
            left_n1->parent = n2;
            n1->left = left_n2;
            n1->right = right_n2;

            if (right_n2)
                right_n2->parent = n1;
            if (parent2 != n1) {
                n2->right = right_n1;
                right_n1->parent = n2;
                parent2->left = n1;
                n1->parent = parent2;
            }
            else {
                n2->right = n1;
                n1->parent = n2;
            }
        }

        node_pointer erase_min(node_pointer n)
        {
            if (!n->left) {
                delete_node(n);
                --_size;
                return NULL;
            }
            if (!is_red(n->left) && !is_red(n->left->left))
                n = move_red_left(n);
            n->left = erase_min(n->left);
            return fix(n);
        }

        void clear(node_pointer node)
        {
            if (!node)
                return ;
            clear(node->left);
            clear(node->right);
            delete_node(node);
        }

        /* UTILITIES */
        node_pointer get_min(node_pointer n)
        {
            while (n->left)
                n = n->left;
            return n;
        }

        node_pointer get_max(node_pointer n)
        {
            while (n->right)
                n = n->right;
            return n;
        }

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

        /*
         * Add description of what it do
         */
        node_pointer move_red_left(node_pointer n)
        {
            flip_colors(n);
            if (n->right && is_red(n->right->left)) {
                n->right = rotate_right(n->right);
                n = rotate_left(n);
                flip_colors(n);
            }
            return n;
        }

        node_pointer move_red_right(node_pointer n)
        {
            flip_colors(n);
            if (n->left && is_red(n->left->left)) {
                n = rotate_right(n);
                flip_colors(n);
            }
            return n;
        }

        /*
         * Rotate node to it's left (right) subtree.
         * The root of it's right (left) subtree takes its place.
         * The new root is returned.
         *
         *      rotate_left(B):
         *
         *      B                    C
         *    /   \      ->         /  \
         *   A     C               B    E
         *       /   \           /   \
         *      D     E         A     D
         */

        node_pointer rotate_left (node_pointer root)
        {
            node_pointer right_root = root->right;

            root->right = right_root->left;
            root->right ? root->right->parent = root : 0;

            right_root->left = root;
            right_root->parent = root->parent;
            root->parent = right_root;

            right_root->color = root->color;
            root->color = RED;
            return right_root;
        }

        node_pointer rotate_right (node_pointer root)
        {
            node_pointer left_root = root->left;

            root->left = left_root->right;
            root->left ? root->left->parent = root : 0;

            left_root->right = root;
            left_root->parent = root->parent;
            root->parent = left_root;

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

        void flip_colors (node_pointer n)
        {
            n->switch_color();
            if (n->left)
                n->left->switch_color();
            if (n->right)
                n->right->switch_color();
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

}; // CLASS LLRB

} // NAMESPACE FT
