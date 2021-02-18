
        /* TRY SHIT OUT */

        node_pointer right_rotate (node_pointer node_to_be_balanced)
        {
            node_pointer temp = node_to_be_balanced->left;
            node_pointer right_child = temp->right;
            temp->right = node_to_be_balanced;
            node_to_be_balanced->left = right_child;
            node_to_be_balanced->height = height_of_node(node_to_be_balanced);
            temp->height = height_of_node(temp);
            return temp;
        }

        node_pointer left_rotate (node_pointer node_to_be_balanced)
        {
            node_pointer temp = node_to_be_balanced->right;
            node_pointer left_child = (node_to_be_balanced->right->left);
            temp->left = node_to_be_balanced;
            node_to_be_balanced->right = left_child;
            node_to_be_balanced->height = height_of_node(node_to_be_balanced);
            temp->height = height_of_node(temp);
            return temp;
        }

        node_pointer left_right_rotate (node_pointer node_to_be_balanced)
        {
            node_to_be_balanced->left = left_rotate (node_to_be_balanced->left);
            return right_rotate(node_to_be_balanced);
        }

        node_pointer right_left_rotate (node_pointer node_to_be_balanced)
        {
            node_to_be_balanced->right = right_rotate (node_to_be_balanced->right);
            return left_rotate(node_to_be_balanced);
        }

        size_t height_of_node (node_pointer root)
        {
            if (!root)
                return 0;
            return std::max(height_of_node(root->left), height_of_node(root->right)) + 1;
        }

        /* END TRY SHIT OUT */
