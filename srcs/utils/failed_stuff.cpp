///* SWAP NODES*/
//std::cout << "DELETE NODE WITH 2 CHILD" << std::endl;
//std::cout << "FOUND MIN AT " << min->pair.first << std::endl;
//
//child->left->parent = min;
//child->right->parent = min;
//
//if (min->right) {
//    min->right->parent = child;
//    child->right = min->right;
//}
//else
//    child->right = NULL;
//// ??
//min->right = child;
//min->left = child->left;
//
//if (parent && parent->left == child)
//    parent->left = min;
//else if (parent && parent->right == child)
//    parent->right = min;
//
//if (min->parent == child)
//    child->parent = min;
//else
//    child->parent = min->parent;
//
//min->parent = parent;
//child->left = NULL;
//
//if (!parent)
//    _root = min;
//
//std::cout << std::endl << "JUST AFTER SWISCHING NODES" << std::endl;
//std::cout << "************************************************" << std::endl;
//print_debug();
//std::cout << "************************************************" << std::endl;


///* DEBUG */
//void print_debug(void) {
//    if (!_root) {
//        std::cout << "******  TREE IS EMPTY ******" << std::endl;
//        return ;
//    }
//    std::cout << "******  TREE IS NOT EMPTY ******" << std::endl;
//    std::cout << "SIZE = " << _size << std::endl;
//    std::cout << "EMPTY = " << std::boolalpha << (_size == 0) << std::endl;
//
//    node_pointer n;
//    ft::queue<node_pointer> q;
//    q.push(_root);
//    while (!q.empty()) {
//        n = q.front();
//        aux_print_debug(n);
//        q.pop();
//        if (n->left)
//            q.push(n->left);
//        if (n->right)
//            q.push(n->right);
//    }
//}
//
//void aux_print_debug(node_pointer n) {
//    std::cout << "NODE {" << n->pair.first << "} ";
//    std::cout << "PARENT {" << (n->parent ? std::to_string(n->parent->pair.first) : "NONE") << "} ";
//    std::cout << "CHILDREN {" << (n->left ? std::to_string(n->left->pair.first) : "NONE") << ", " << (n->right ? std::to_string(n->right->pair.first) : "NONE") << "}" << std::endl;
//}
