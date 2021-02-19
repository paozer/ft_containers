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
