#pragma once

#include <cstddef> // size_t
#include <memory> // std::allocator
#include <functional> // std::less
#include <cmath> // std::abs

#include "utils.hpp" // ft::max
#include "avl_node.hpp"

/* DEBUG */
#include "../queue/queue.hpp"
#include <iostream>

namespace ft {

template <class Key,
          class T,
          class Compare = std::less<Key>,
          class Alloc = std::allocator<std::pair<const Key, T> >
          >
class avl_tree
{
    public:
        typedef Key key_type;
        typedef T mapped_type;
        typedef std::pair<const key_type, mapped_type> value_type;
        typedef avl_node<key_type, mapped_type> node;
        typedef node * node_pointer;
        typedef Compare key_compare;
        typedef Alloc allocator_type;
        typedef size_t size_type;

    private:
        typedef typename Alloc::template rebind<node>::other node_allocator;

    public:
        avl_tree (const allocator_type& alloc = allocator_type())
            : _root(NULL), _size(0), _added_node_flag(false), _added_node_ptr(NULL),
              _alloc(alloc), _comp(key_compare())
        {
        }

        ~avl_tree() { clear(); }

        /* ELEMENT ACCESS */

        /* CAPACITY */
        size_type size(void) const { return _size; }
        bool empty(void) const { return _size == 0; }
        size_type max_size(void) { return _alloc.max_size(); }

        /* MODIFIERS */
        std::pair<node_pointer, bool> insert(const value_type& val)
        {
            _added_node_flag = false;
            _added_node_ptr = NULL;
            if (_root)
                aux_insert(_root, val);
            else {
                _root = new_node(val);
                ++_size;
                return std::make_pair(_root, true);
            }
            return std::make_pair(_added_node_ptr, _added_node_flag);
        }

        node_pointer insert (node_pointer position, const value_type& val)
        {
            (void)position;
            (void)val;
            return NULL;
        }

        void clear(void)
        {
            aux_clear(_root);
        }

        size_type erase (const key_type& key)
        {
            if (_root)
                aux_erase(NULL, _root, key);
            return 0;
        }

    private:
        node_pointer _root;
        size_type _size;
        bool _added_node_flag;
        node_pointer _added_node_ptr;
        node_allocator _alloc;
        key_compare _comp;

        /* INSERT */
        void aux_insert(node_pointer parent, const value_type& val)
        {
            if (_comp(parent->pair.first, val.first)) {
                if (!parent->right) {
                    parent->right = new_node(val);
                    parent->right->parent = parent;
                    ++_size;
                    _added_node_ptr = parent->right;
                    _added_node_flag = true;
                }
                else
                    aux_insert(parent->right, val);
            }
            else if (_comp(val.first, parent->pair.first)) {
                if (!parent->left) {
                    parent->left = new_node(val);
                    parent->left->parent = parent;
                    ++_size;
                    _added_node_ptr = parent->left;
                    _added_node_flag = true;
                }
                else
                    aux_insert(parent->left, val);
            }
            else {
                _added_node_ptr = parent;
                _added_node_flag = false;
                return ;
            }
            check_balance(parent);
        }

        void rebalance(node_pointer node, int bf)
        {
            // if left child imbalanced
            // else right child imbalanced
            if (bf > 1) {
                // if left subtree -> R Rotation
                // else right subtree -> LR Rotation
                if (height(node->left->left) > height(node->left->right))
                    node = right_rotate(node);
                else
                    node = left_right_rotate(node);
            }
            else {
                // if right subtree -> L Rotation
                // else left subtree -> RL Rotation
                if (height(node->right->right) > height(node->right->left))
                    node = left_rotate(node);
                else
                    node = right_left_rotate(node);
            }
            if (!node->parent)
                _root = node;
        }

        /* ERASE */
        void aux_erase(node_pointer parent, node_pointer child, const key_type& key)
        {
            if (!child)
                return ;
            if (_comp(child->pair.first, key))
                aux_erase(child, child->right, key);
            else if (_comp(key, child->pair.first))
                aux_erase(child, child->left, key);
            else {
                if (!child->left && !child->right)
                    aux_erase_no_child_node(parent, child);
                else if ((!child->left && child->right) || (child->left && !child->right))
                    aux_erase_one_child_node(parent, child);
                else {
                    aux_erase_two_child_node(child, key);
                    return ;
                }
                --_size;
            }
            check_balance(parent);
        }

        void aux_erase_no_child_node(node_pointer parent, node_pointer child)
        {
            if (parent) {
                if (parent->left == child)
                    parent->left = NULL;
                else if (parent->right == child)
                    parent->right = NULL;
            }
            else
                _root = NULL;
            delete_node(child);
        }

        void aux_erase_one_child_node(node_pointer parent, node_pointer child)
        {
            if (parent) {
                if (parent->left == child)
                    parent->left = (child->left ? child->left : child->right);
                else if (parent->right == child)
                    parent->right = (child->left ? child->left : child->right);
            }
            if (child->left)
                child->left->parent = parent;
            else
                child->right->parent = parent;
            delete_node(child);
        }

        void aux_erase_two_child_node(node_pointer child, const key_type& key)
        {
            node_pointer min = get_min(child->right);
            swap_nodes(child, min);
            aux_erase(min, min->right, key);
        }

        /* UTILITIES */
        void check_balance(node_pointer node)
        {
            if (!node)
                return ;
            int bf = height(node->left) - height(node->right);
            if (std::abs(bf) > 1)
                rebalance(node, bf);
            if (!node->parent)
                return ;
            check_balance(node->parent);
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

        void aux_clear(node_pointer node)
        {
            if (!node)
                return ;
            aux_clear(node->left);
            aux_clear(node->right);
            delete_node(node);
        }

        int height(node_pointer node)
        {
            if (!node)
                return 0;
            return ft::max(height(node->left), height(node->right)) + 1;
        }

        node_pointer get_min(node_pointer node)
        {
            while (node->left)
                node = node->left;
            return node;
        }

        /* ROTATIONS */
        node_pointer left_rotate(node_pointer node)
        {
            node_pointer tmp = node->right;
            node->right = tmp->left;
            node->right ? node->right->parent = node : 0;
            tmp->left = node;
            tmp->parent = node->parent;
            if (node->parent && node->parent->left == node)
                node->parent->left = tmp;
            else if (node->parent && node->parent->right == node)
                node->parent->right = tmp;
            node->parent = tmp;
            return tmp;
        }

        node_pointer right_rotate(node_pointer node)
        {
            node_pointer tmp = node->left;
            node->left = tmp->right;
            node->left ? node->left->parent = node : 0;
            tmp->right = node;
            tmp->parent = node->parent;
            if (node->parent && node->parent->left == node)
                node->parent->left = tmp;
            else if (node->parent && node->parent->right == node)
                node->parent->right = tmp;
            node->parent = tmp;
            return tmp;
        }

        node_pointer right_left_rotate(node_pointer node)
        {
            node->right = right_rotate(node->right);
            return left_rotate(node);
        }

        node_pointer left_right_rotate(node_pointer node)
        {
            node->left = left_rotate(node->left);
            return right_rotate(node);
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

}; // CLASS AVL_TREE

} // NAMESPACE FT
