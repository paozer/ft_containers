#pragma once

#include "utils.hpp"
#include "avl_node.hpp"

#include <cstddef> // size_t
#include <cmath> // std::abs

#include <memory> // std::allocator
#include <functional> // std::less

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
        /* CONSTRUCTORS */
        avl_tree (const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
            : _root(NULL), _begin(new_node()), _end(new_node()), _added_node_ptr(NULL),
              _added_node(false), _size(0), _alloc(alloc), _comp(comp)
        {
            _begin->parent = _end;
            _end->parent = _begin;
        }

        /* DESTRUCTOR */
        ~avl_tree()
        {
            unset_bounds();
            clear();
            delete_node(_begin);
            delete_node(_end);
        }

        /* ELEMENT ACCESS */
        node_pointer begin (void) const { return _begin->parent; }
        node_pointer end (void) const { return _end; }
        node_pointer rbegin (void) const { return _begin; }
        node_pointer rend (void) const { return _end->parent; }

        /* CAPACITY */
        size_type size (void) const { return _size; }
        bool empty (void) const { return _size == 0; }
        size_type max_size (void) const { return _alloc.max_size(); }

        /* MODIFIERS */
        std::pair<node_pointer, bool> insert (const value_type& val)
        {
            if (_root) {
                unset_bounds();
                _root = aux_insert(_root->parent, _root, val);
            }
            else
                _root = aux_insert(NULL, NULL, val);
            if (_added_node)
                set_bounds();
            return std::make_pair(_added_node_ptr, _added_node);
        }

        std::pair<node_pointer, bool> insert (node_pointer hint, const value_type& val)
        {
            return insert(val);
        }

        void clear (void)
        {
            if (_size == 0)
                return ;
            unset_bounds();
            aux_clear(_root);
            _size = 0;
            _root = NULL;
            set_bounds();
        }

        size_type erase (const key_type& key)
        {
            if (_size == 0)
                return 0;
            unset_bounds();
            size_type old_size = _size;
            aux_erase(NULL, _root, key);
            set_bounds();
            return old_size - _size;
        }

        size_type erase (node_pointer node)
        {
            if (_size == 0)
                return 0;
            unset_bounds();
            size_type old_size = _size;
            aux_erase(node->parent, node, node->pair.first);
            set_bounds();
            return old_size - _size;
        }

        /* OPERATIONS */
        node_pointer find (const key_type& key) const
        {
            node_pointer node = _root;
            while (node && node != _begin && node != _end) {
                if (_comp(node->pair.first, key))
                    node = node->right;
                else if (_comp(key, node->pair.first))
                    node = node->left;
                else
                    return node;
            }
            return NULL;
        }

    private:
        node_pointer _root;
        node_pointer _begin;
        node_pointer _end;
        node_pointer _added_node_ptr;
        bool _added_node;
        size_type _size;
        node_allocator _alloc;
        key_compare _comp;

        /* INSERT */
        node_pointer aux_insert (node_pointer parent, node_pointer node, const value_type& val)
        {
            // if we're at a leaf insert key/val pair
            // else if key goes into the left subtree
            // else if key goes into the right subtree
            // else key is already in the tree
            // remember the added/found node to create iterator after return from recursion
            if (!node) {
                node = new_node(val);
                node->parent = parent;
                _added_node = true;
                _added_node_ptr = node;
                ++_size;
            }
            else if (_comp(val.first, node->pair.first))
                node->left = aux_insert(node, node->left, val);
            else if (_comp(node->pair.first, val.first))
                node->right = aux_insert(node, node->right, val);
            else {
                _added_node = false;
                _added_node_ptr = node;
            }
            return node;
        }

        /* ERASE */
        void aux_erase (node_pointer parent, node_pointer child, const key_type& key)
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
                    // will decrement size in aux_erase_two_child_node function
                    aux_erase_two_child_node(child, key);
                    return ;
                }
                --_size;
            }
            fix_up(parent);
        }

        void aux_erase_no_child_node (node_pointer parent, node_pointer child)
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

        void aux_erase_one_child_node (node_pointer parent, node_pointer child)
        {
            if (parent) {
                if (parent->left == child)
                    parent->left = (child->left ? child->left : child->right);
                else if (parent->right == child)
                    parent->right = (child->left ? child->left : child->right);
            }
            else
                _root = (child->left ? child->left : child->right);
            if (child->left)
                child->left->parent = parent;
            else
                child->right->parent = parent;
            delete_node(child);
        }

        void aux_erase_two_child_node (node_pointer child, const key_type& key)
        {
            node_pointer min = get_min(child->right);
            swap_nodes(child, min);
            aux_erase(min, min->right, key);
        }

        /* UTILITIES */
        void fix_up (node_pointer node)
        {
            if (!node)
                return ;
            // while node is unbalanced
            while (node->bf != 0) {
                // if left child imbalanced
                // else right child imbalanced
                if (node->bf == -2) {
                    // if left subtree -> R Rotation
                    // else right subtree -> LR Rotation
                    if (node->left->bf == -1) {
                        node->bf = 0;
                        node->left->bf = 0;
                        node = right_rotate(node);
                    }
                    else {
                        int lr_bf = node->left->right->bf;
                        node->bf = 0;
                        node->left->bf = 0;
                        node->left->right->bf = 0;
                        if (lr_bf == 1)
                            node->left->bf = -1;
                        else if (lr_bf == -1)
                            node->bf = 1;
                        node = left_right_rotate(node);
                    }
                    break ;
                }
                else if (node->bf == 2) {
                    if (node->right->bf == 1) {
                        node->bf = 0;
                        node->right->bf = 0;
                        node = left_rotate(node);
                    }
                    else {
                        int rl_bf = node->right->left->bf;
                        node->bf = 0;
                        node->right->bf = 0;
                        node->right->left->bf = 0;
                        if (rl_bf == 1)
                            node->bf = -1;
                        else if (rl_bf == -1)
                            node->right->bf = 1;
                        node = right_left_rotate(node);
                    }
                    break ;
                }
                if (!node->parent)
                    return ;
                else if (node->parent->left == node)
                    ++node->parent->bf;
                else
                    --node->parent->bf;
                node = node->parent;
            }
        }

        void unset_bounds (void)
        {
            if (_begin->parent != _end)
                _begin->parent->left = NULL;
            if (_end->parent != _begin)
                _end->parent->right = NULL;
        }

        void set_bounds (void)
        {
            if (_size != 0) {
                node_pointer min = get_min(_root);
                node_pointer max = get_max(_root);
                _begin->parent = min;
                _begin->parent->left = _begin;
                _end->parent = max;
                _end->parent->right = _end;
            }
            else {
                _begin->parent = _end;
                _end->parent = _begin;
            }
        }

        void swap_nodes (node_pointer n1, node_pointer n2)
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

        void aux_clear (node_pointer node)
        {
            if (!node)
                return ;
            aux_clear(node->left);
            aux_clear(node->right);
            delete_node(node);
        }

        node_pointer get_min (node_pointer node)
        {
            if (!node)
                return NULL;
            while (node->left)
                node = node->left;
            return node;
        }

        node_pointer get_max (node_pointer node)
        {
            if (!node)
                return NULL;
            while (node->right)
                node = node->right;
            return node;
        }

        /* ROTATIONS */
        node_pointer left_rotate (node_pointer node)
        {
            node_pointer tmp = node->right;
            node->right = tmp->left;
            node->right ? node->right->parent = node : 0;
            tmp->left = node;
            tmp->parent = node->parent;
            if (!node->parent)
                _root = tmp;
            else if (node->parent->left == node)
                node->parent->left = tmp;
            else
                node->parent->right = tmp;
            node->parent = tmp;
            return tmp;
        }

        node_pointer right_rotate (node_pointer node)
        {
            node_pointer tmp = node->left;
            node->left = tmp->right;
            node->left ? node->left->parent = node : 0;
            tmp->right = node;
            tmp->parent = node->parent;
            if (!node->parent)
                _root = tmp;
            else if (node->parent->left == node)
                node->parent->left = tmp;
            else
                node->parent->right = tmp;
            node->parent = tmp;
            return tmp;
        }

        node_pointer right_left_rotate (node_pointer node)
        {
            node->right = right_rotate(node->right);
            return left_rotate(node);
        }

        node_pointer left_right_rotate (node_pointer node)
        {
            node->left = left_rotate(node->left);
            return right_rotate(node);
        }

        /* MEMORY MANAGEMENT */
        node_pointer new_node (const value_type& val = value_type())
        {
            node_pointer p = _alloc.allocate(1);
            _alloc.construct(p, val);
            return p;
        }

        void delete_node (node_pointer p)
        {
            _alloc.destroy(p);
            _alloc.deallocate(p, 1);
        }

}; // CLASS AVL_TREE

} // NAMESPACE FT
